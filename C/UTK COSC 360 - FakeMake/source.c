#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fields.h"
#include "dllist.h"

void add_fields_to_dllist(Dllist list, IS is) {
    for (int i = 1; i < is->NF; i++) {
        dll_append(list, new_jval_s(strdup(is->fields[i])));
    }
}

time_t process_header_files(Dllist list) {
    Dllist tmp;
    struct stat buf;
    time_t max_mtime = 0;

    dll_traverse(tmp, list) {
        if (stat(tmp->val.s, &buf) == -1) {
            fprintf(stderr, "Error: No %s file found to stat.\n", tmp->val.s);
            exit(1);
        }
        if (buf.st_mtime > max_mtime) {
            max_mtime = buf.st_mtime;
        }
    }

    // printf("Maximum modification time for header files: %ld\n", max_mtime);
    return max_mtime;
}

void process_c_files(Dllist C_files, time_t header_mtime, char *executable_name, Dllist L_files, Dllist F_files) {
    Dllist tmp_c;
    Dllist tmp_f;
    Dllist tmp_l;
    struct stat c_buf, o_buf;
    time_t max_mtime = 0;
    int did_c_file_compile = 0;

    static char *compiled_files[256];
    static int compiled_count = 0;

    // Arrays to store the object files and other files in the order they were read
    char *ordered_files[256];
    int ordered_count = 0;

    // Arrays for L_files and F_files
    char *l_files[256];
    int l_files_count = 0;

    char *f_files[256];
    int f_files_count = 0;

    dll_traverse(tmp_f, F_files) {
        f_files[f_files_count++] = strdup(tmp_f->val.s);
    }

    // Traverse L_files and store them in the array
    dll_traverse(tmp_l, L_files) {
        l_files[l_files_count++] = strdup(tmp_l->val.s);
    }

    // Traverse the list in order of C files (they should appear in the description file order)
    dll_traverse(tmp_c, C_files) {
        if (stat(tmp_c->val.s, &c_buf) == -1) {
            fprintf(stderr, "Error: No %s file found to stat.\n", tmp_c->val.s);
            exit(1);
        }
        if (c_buf.st_mtime > max_mtime) {
            max_mtime = c_buf.st_mtime;
        }
    
        char o_file[256];
        char *dot = strrchr(tmp_c->val.s, '.');
        if (dot) {
            *dot = '\0';
        }
        snprintf(o_file, sizeof(o_file), "%s.o", tmp_c->val.s);
        
        
        // Check the command for compilation
        if (stat(o_file, &o_buf) == -1 || o_buf.st_mtime < c_buf.st_mtime || o_buf.st_mtime < header_mtime){
        // Initialize the gcc command with the base part
            char gcc_command[1024];
            snprintf(gcc_command, sizeof(gcc_command), "gcc -c");  // Start with the base gcc -c command
                    
            // Append the flags from f_files to the command
            for (int i = 0; i < f_files_count; i++) {
                // Append each flag from f_files
                snprintf(gcc_command + strlen(gcc_command), sizeof(gcc_command) - strlen(gcc_command), " %s", f_files[i]);
            }
            
            // Finally, append the C file to the command
            snprintf(gcc_command + strlen(gcc_command), sizeof(gcc_command) - strlen(gcc_command), " %s.c", tmp_c->val.s);
            
            // Now gcc_command has all the necessary parts
            printf("%s\n", gcc_command);  // Print the command for debugging
            
            if (system(gcc_command) != 0) {
                fprintf(stderr, "Command failed.  Exiting\n");
                exit(1);
            } else {
                did_c_file_compile = 1;
            }
        }

        // Add the object file name in the correct order
        ordered_files[ordered_count++] = strdup(o_file); // Keep track of the object files in order
    }

    // Traverse F_files and store them in the array
   

    // After processing all files, ensure that we try to link the executable
    if (ordered_count > 0 && stat(executable_name, &o_buf) != -1 && max_mtime < o_buf.st_mtime && !did_c_file_compile) {
        printf("%s up to date\n", executable_name);
    } else if (ordered_count > 0 || did_c_file_compile) {
        char final_command[1024];
        snprintf(final_command, sizeof(final_command), "gcc -o %s", executable_name);
        
        // Append each ordered file to the command string
        for (int s = 0; s <f_files_count; s++) {
            strcat(final_command, " ");
            strcat(final_command, f_files[s]);
        }

        for (int i = 0; i < ordered_count; i++) {
            strcat(final_command, " ");
            strcat(final_command, ordered_files[i]);
        }

        for (int k = 0; k < l_files_count; k++) {
            strcat(final_command, " ");
            strcat(final_command, l_files[k]);
        }

        printf("%s\n", final_command); // Print the final gcc command
        if(system(final_command) != 0){
            fprintf(stderr, "Command failed.  Fakemake exiting\n");
            exit(1);
        }; // Call system() to link the final executable
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < ordered_count; i++) {
        free(ordered_files[i]); // Free the memory for object file names
    }
    for (int i = 0; i < f_files_count; i++) {
        free(f_files[i]); // Free the memory for F_files
    }
    for (int i = 0; i < l_files_count; i++) {
        free(l_files[i]); // Free the memory for L_files
    }
}

int main(int argc, char **argv) {
    const char *fmakefile = "fmakefile"; // Define the default file name
    const char *description_file; // Declare the description_file pointer
    IS is;
    int line_number = 0;
    Dllist C_files = new_dllist();
    Dllist H_files = new_dllist();
    Dllist L_files = new_dllist(); // -lm <name> 
    Dllist F_files = new_dllist(); //flags 

    int only_one_executable = 0;
    char *executable_name = NULL;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s <description_file> ...\n", argv[0]);
        exit(1);
    }

    // argv[1] is the description file
    if (argc != 2) {
        description_file = fmakefile;
    } else {
        description_file = argv[1];
    }

    is = new_inputstruct(description_file);
    if (is == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", description_file);
        exit(1);
    }

    while (get_line(is) >= 0) {
        line_number++;
        if (is->text1[0] == 'C') {
            // printf("%d: %s", line_number, is->text1);
            add_fields_to_dllist(C_files, is);
            // printf("\n");
        }
        else if (is->text1[0] == 'H') {
            // printf("%d: %s", line_number, is->text1);
            add_fields_to_dllist(H_files, is);
            // printf("\n");
        }
        else if (is->text1[0] == 'L') {
            // printf("%d: %s", line_number, is->text1);
            add_fields_to_dllist(L_files, is);
            // printf("\n");
        }
        else if (is->text1[0] == 'F') {
            // printf("%d: %s", line_number, is->text1);
            add_fields_to_dllist(F_files, is);
            // printf("\n");
        }
        else if (is->text1[0] == 'E') { //only one executable per description file
            if (only_one_executable) {
                fprintf(stderr, "Error: Only one executable allowed per description file\n");
                exit(1);
            } else {
                only_one_executable = 1;
                executable_name = strdup(is->fields[1]);
            }
        } 
        else if (is->text1[0] != '#' && is->text1[0] != '\n' && is->text1[0] != '\0') {
            fprintf(stderr, "Error: Invalid line at %d: %s", line_number, is->text1);
            exit(1);
        }
    }
    if (!only_one_executable) {
        fprintf(stderr, "No executable specified\n");
        exit(1);
    }
    
    int header_mtime = process_header_files(H_files);
    process_c_files(C_files, header_mtime, executable_name, L_files, F_files);

    free(executable_name);
    
    jettison_inputstruct(is);

    return 0;
}

