#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int bitstream_size = 0;
int bitstream_check = 0;

// Huffman Tree Node
typedef struct node {
    struct node *zero;
    struct node *one;
    char *s;
} Node;

// Create a new node
Node *create_node() {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        perror("Memory allocation failed");
        exit(1);
    }
    node->zero = NULL;
    node->one = NULL;
    node->s = NULL;
    return node;
}

// Insert a character with its binary sequence into the tree
void insert(Node *root, const char *sequence, const char *character) {
    Node *current = root;
    while (*sequence) {
        if (*sequence == '0') {
            if (!current->zero)
                current->zero = create_node();
            current = current->zero;
        } else if (*sequence == '1') {
            if (!current->one)
                current->one = create_node();
            current = current->one;
        }
        sequence++;
    }
    current->s = strdup(character);
}



// Free the Huffman tree
void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->zero);
    free_tree(root->one);
    free(root->s);
    free(root);
}

void save_bitstream_length(int num) {
    bitstream_size = num;
    bitstream_check = num;
}

void reverseArray(int arr[], int n) { // geeks for geeks 
    // Temporary array to store elements in reversed order
    int temp[n];

    // Copy elements from original array to temp in reverse order
    for (int i = 0; i < n; i++)
        temp[i] = arr[n - i - 1];

    // Copy elements back to original array
    for (int i = 0; i < n; i++)
        arr[i] = temp[i];
}
int* process_bitstream(unsigned char *encoded, long file_size, int bitstream_size) {
    int *bitstream_binary = (int*)malloc((bitstream_size + 1) * sizeof(int)); // +1 for the -1 terminator
    if (!bitstream_binary) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int iter = 0;
    for (long i = 0; i < file_size; i++) {
        unsigned char byte = encoded[i];
        int temp[8] = {0};
        for (int j = 7; j >= 0; j--) {
            temp[7 - j] = (byte >> j) & 1;
        }
        reverseArray(temp, 8);
        for (int j = 0; j < 8 && iter < bitstream_size; j++) {
            bitstream_binary[iter++] = temp[j];
        }
    }
    bitstream_binary[iter] = -1; // Terminate the bitstream with -1
    
    return bitstream_binary;
}

void filter_and_process_encoded_data(unsigned char *encoded, int bytes_read, long *file_size) {
    // Extract the bitstream length from the last four bytes (little endian)
    uint32_t bitstream_length = 0;
    for (int i = 0; i < 4; i++) {
        bitstream_length |= (encoded[bytes_read - 4 + i] << (i * 8));
    }

    // Calculate the number of bytes that contain the bitstream
    int bitstream_bytes = (bitstream_length + 7) / 8;

    // Copy the bitstream bytes to the beginning of the encoded array
    for (int i = 0; i < bitstream_bytes; i++) {
        encoded[i] = encoded[i];
    }

    *file_size = bitstream_bytes;
    save_bitstream_length(bitstream_length);
}

void store_code_definition(Node *root, char *chars, char *binary_sequence) {
    insert(root, binary_sequence, chars); //insert into tree
}

void custom_strlen(Node *root, const unsigned char *code_def, int max_length) {
    int i = 0;
    int pair_count = 0;

    while (i < max_length) {
        // Extract characters (string until null terminator)
        int char_start = i;
        while (i < max_length && code_def[i] != 0x00) {
            i++;
        }
        int char_length = i - char_start;
        i++; // Skip the null terminator

        if (char_length == 0) continue; // Skip empty entries

        char *chars = (char *)malloc(char_length + 1);
        for (int j = 0; j < char_length; j++) {
            chars[j] = (code_def[char_start + j] == 0x0A) ? '\n' : 
                       (code_def[char_start + j] == 0x20) ? ' ' : 
                       code_def[char_start + j];  // Replace `0A` with `\n` and `0x20` with space
        }
        chars[char_length] = '\0';
        pair_count++;

        // Extract binary sequence (string until null terminator)
        int bin_start = i;
        while (i < max_length && code_def[i] != 0x00) {
            i++;
        }
        int bin_length = i - bin_start;
        i++; // Skip the null terminator

        if (bin_length == 0) {
            free(chars);
            continue; // Skip empty entries
        }

        char *binary_sequence = (char *)malloc(bin_length + 1);
        strncpy(binary_sequence, (char *)&code_def[bin_start], bin_length);
        binary_sequence[bin_length] = '\0';
        pair_count++;

        // Call store_code_definition when pair_count reaches 2 (one char + one binary sequence)
        if (pair_count == 2) {
            store_code_definition(root, chars, binary_sequence);
            pair_count = 0; // Reset after storing
        }

        free(chars);
        free(binary_sequence);
    }
}

void decode(Node *root, const int *bitstream) {
    Node *current = root;
    int bits_processed = 0; // Keep track of the number of bits decoded

    while (*bitstream != -1 && bits_processed < bitstream_check) {  // Stop if the limit is reached
        if (*bitstream == 0) {
            if (current->zero) {
                current = current->zero;
            } else {
                return;
            }
        } else if (*bitstream == 1) {
            if (current->one) {
                current = current->one;
            } else {
                return;
            }
        }

        // If a leaf node is reached, print and reset
        if (current->s) {
            printf("%s", current->s);
            current = root;  // Reset to root after decoding a symbol
        }

        bitstream++;
        bits_processed++; // Increment the bit counter
    }
}



int main(int argc, char **argv) {
    // Check if sufficient arguments are passed
    Node *root = create_node();
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(1);
    }

    // Loop over all files passed as arguments
    for (int i = 1; i < argc; i++) {
        // printf("Processing file: %s\n", argv[i]);

        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            perror("Error opening file");
            continue; // Skip to the next file if one fails
        }

        // Move the file pointer to the end of the file to get the file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET); // Move the file pointer back to the beginning

        // Allocate a buffer for the encoded data
        unsigned char *encoded = (unsigned char *)malloc(4 * file_size);
        if (encoded == NULL) {
            perror("Error allocating memory");
            fclose(file);
            continue;
        }

        // Read the entire file into the buffer
        int bytes_read = fread(encoded, 1, file_size, file);
        if (bytes_read != (int)file_size) {
            perror("Error reading file");
            free(encoded);
            fclose(file);
            continue;
        }
        int *bitstream = NULL;
        if (i == 1) { 
            custom_strlen(root, encoded, bytes_read);
        }

        if (i == 2) {
            filter_and_process_encoded_data(encoded, bytes_read, &file_size);

            // Call the function to process the bitstream only on second file
            bitstream = process_bitstream(encoded, file_size, bitstream_size);
        }

        if (bitstream != NULL) {
            decode(root, bitstream);
            free(bitstream); // Free the allocated memory for the bitstream
        }

        free(encoded);
        // printf("Closing file %s\n", argv[i]);
        fclose(file);
        // printf("\n");
    }

    return 0;
}