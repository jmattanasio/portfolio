#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <utime.h>
#include "jrb.h"
#include "dllist.h"
#include "unistd.h"

// A structure for file or directory info
typedef struct Node {
    int name_length, mode;
    long inode, mtime, size;
    char filename[256];
    struct Node *link; // Link to another file if it's a hard link
    unsigned char* contents; // File data
} Node;

// Reads file details and fills a Node structure
int read_next_file(Node *node, JRB inodes) {
    int name_length, bytes_read;

    // Get the name length
    bytes_read = fread(&name_length, sizeof(int), 1, stdin);
    if (bytes_read != 1) return -1; // End of file

    node->name_length = name_length;

    // Check if name length is valid
    if (name_length <= 0) {
        fprintf(stderr, "Bad tarc file. File name size is %d, which I can't malloc.\n", name_length);
        exit(1);
    }

    char filename[256];
    bytes_read = fread(&filename, sizeof(char), name_length, stdin);
    if (bytes_read != name_length) {
        fprintf(stderr, "Bad tarc file. Couldn't read name\n");
        exit(1);
    }

    filename[name_length] = '\0';
    strcpy(node->filename, filename);

    // Read the inode (file identifier)
    long inode;
    bytes_read = fread(&inode, sizeof(long), 1, stdin);
    if (bytes_read != 1) {
        fprintf(stderr, "Bad tarc file for %s. Couldn't read inode\n", node->filename);
        exit(1);
    }
    node->inode = inode;

    // Check if this inode already exists (indicating a hard link)
    JRB inode_jrb = jrb_find_int(inodes, inode);
    if (inode_jrb != NULL) {
        Node *linked_node = (Node*) jrb_val(inode_jrb).v;
        node->link = linked_node; // Link this node to the existing one
        return 0;
    }

    // Otherwise, add this inode to the tree
    node->link = NULL;
    jrb_insert_int(inodes, inode, new_jval_v(node));

    // Read the file's permissions
    int mode;
    bytes_read = fread(&mode, sizeof(int), 1, stdin);
    if (bytes_read != 1) {
        fprintf(stderr, "Bad tarc file for %s. Couldn't read mode\n", node->filename);
        exit(1);
    }
    node->mode = mode;

    // Read the file's modification time
    long mtime;
    bytes_read = fread(&mtime, sizeof(long), 1, stdin);
    if (bytes_read != 1) {
        fprintf(stderr, "Bad tarc file for %s. Couldn't read mtime\n", node->filename);
        exit(1);
    }
    node->mtime = mtime;

    // If it's a directory, stop reading data
    if (S_ISDIR(mode)) return 0;

    // Read the file size
    long size;
    bytes_read = fread(&size, sizeof(long), 1, stdin);
    if (bytes_read != 1) {
        fprintf(stderr, "Bad tarc file for %s. Couldn't read size\n", node->filename);
        exit(1);
    }
    node->size = size;

    // Check if the size is valid
    if (size <= 0 || size >= 100000000) {
        fprintf(stderr, "Bad tarc file for %s. Invalid file size %ld\n", node->filename, size);
        exit(1);
    }

    // Read the file contents into a buffer
    unsigned char buf[size];
    bytes_read = fread(&buf, sizeof(unsigned char), size, stdin);
    if (bytes_read != size) {
        fprintf(stderr, "Bad tarc file for %s. Couldn't read contents\n", node->filename);
        exit(1);
    }
    node->contents = (unsigned char*) malloc(sizeof(unsigned char) * size);
    memcpy(node->contents, buf, size);
}

// Reads all files from the archive and sorts them into files and directories
void read_all_files(JRB inodes, Dllist files, Dllist dirs) {
    int count = 0;
    Node *node;
    while (1) {
        node = (Node*) malloc(sizeof(Node));
        int status = read_next_file(node, inodes);
        if (status == -1) {
            free(node);
            break;
        }

        // Add node to the appropriate list (files or directories)
        if (S_ISDIR(node->mode)) {
            dll_append(dirs, new_jval_v(node));
        } else {
            dll_append(files, new_jval_v(node));
        }
        count++;
    }
}

// Creates all directories first
void create_directories(Dllist dirs) {
    Dllist tmp;
    dll_traverse(tmp, dirs) {
        Node* node = (Node*) tmp->val.v;
        int status = mkdir(node->filename, S_IRWXU | S_IRWXG | S_IRWXO);
        if (status != 0) {
            perror("mkdir failed");
        }
    }
}

// Creates files and links, then writes their contents and sets permissions
void create_files_and_links(Dllist files) {
    Dllist tmp;
    dll_traverse(tmp, files) {
        Node* node = (Node*) tmp->val.v;

        // Handle hard links
        if (node->link != NULL) {
            int link_status = link(node->link->filename, node->filename);
            if (link_status != 0) {
                perror("Link creation failed");
            }
            continue;
        }

        // Create the file and write its contents
        FILE *file = fopen(node->filename, "wb");
        if (file == NULL) {
            perror("File creation failed");
            continue;
        }

        size_t bytes_written = fwrite(node->contents, sizeof(unsigned char), node->size, file);
        if (bytes_written != node->size) {
            perror("File write error");
        }
        fclose(file);

        // Set the file's permissions
        chmod(node->filename, node->mode);

        // Set the file's timestamps
        struct utimbuf times;
        times.modtime = node->mtime;
        times.actime = node->mtime;
        utime(node->filename, &times);
    }
}

// Frees memory used by files
void free_files(Dllist files) {
    Dllist tmp;
    dll_traverse(tmp, files) {
        Node* node = (Node*) tmp->val.v;
        if (node->contents != NULL) {
            free(node->contents);
        }
        free(node);
    }
}

// Updates permissions and timestamps for directories
void mode_and_times(Dllist dirs) {
    Dllist tmp;
    dll_traverse(tmp, dirs) {
        Node* node = (Node*) tmp->val.v;
        int chmod_status = chmod(node->filename, node->mode);
        if (chmod_status != 0) {
            perror("chmod failed");
        }

        struct utimbuf times;
        times.modtime = node->mtime;
        times.actime = node->mtime;
        int utime_status = utime(node->filename, &times);
        if (utime_status != 0) {
            perror("utime failed");
        }
    }
}

int main(int argc, char **argv) {
    JRB inodes = make_jrb(); // Track inodes for hard links
    Dllist files = new_dllist();
    Dllist dirs = new_dllist();

    int file_count = 0;
    int dir_count = 0;

    // Read all files and directories
    read_all_files(inodes, files, dirs);

    // Count files and directories
    Dllist tmp;
    dll_traverse(tmp, files) {
        file_count++;
    }
    dll_traverse(tmp, dirs) {
        dir_count++;
    }

    // Create directories first, then files
    create_directories(dirs);
    create_files_and_links(files);

    // Free memory used by files
    free_files(files);

    // Update directory permissions and timestamps
    mode_and_times(dirs);

    // Clean up
    free_dllist(files);
    free_dllist(dirs);
    jrb_free_tree(inodes);
}