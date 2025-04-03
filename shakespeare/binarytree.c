//Includes
#include <stdio.h> //c standard
#include <getopt.h> //programm argument processing
#include <string.h> //string copy
#include <stdlib.h> //for constants
#include <unistd.h> //filemanagement
#include <time.h> //time for randomfunction

#ifndef BINTREE
#include "binarytree.h"
#endif

//Macros
#define FALSE 0
#define TRUE !(FALSE)
#define ERROR_FILEREAD(message) fprintf(stderr,"%s is not a valid input file name!\n",message)
#define ERROR_FILEOPEN(message) fprintf(stderr,"An error occured while opening %s!\n",message)
#define ERROR_USAGE(message) fprintf(stderr,"Usage: %s [-h] [-s <n>] [-l <m>] [-f <filename>]\n",message)
#define ERROR_NAMELESS(message) fprintf(stderr,"There is no name %s\n",message)


//just print out a usage() message on stderr
void usage(char* filename){
    if (filename != "\0") {
        ERROR_USAGE(filename);
    }
    else {
        ERROR_NAMELESS("...");
    }
}

//Tries to open a file with a given filename; return: pointer to FILE or NULL
FILE* openInputFile(char* filename) {
    FILE* inputFile = stdin;
    if (*filename != '\0') { // Ensure filename is not empty
        if ((inputFile = fopen(filename, "r")) == NULL){
            ERROR_FILEOPEN(filename); // could not open for read
        }
    }
    return inputFile;
}

//Checks if the file is readable and prints an error if not; return: (TRUE for readable, FALSE for not)
int isReadableFile(char* filename) {
    int returnBoolean;
    if (access(filename, R_OK) == EXIT_SUCCESS) { // File is readable
        returnBoolean = TRUE;
    } else {
        returnBoolean = FALSE;
        ERROR_FILEREAD(filename);   
    }
    return returnBoolean;
}

//Function to add the following character of my node string to a linked list
void add_char_to_list(LNode **head, char character) {
    for (LNode *current = *head; current != NULL; current = current->next) {
        if (current->character == character) {
            current->count++;
            return;
        }
    }
    LNode *newNode = malloc(sizeof(LNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->character = character;
    newNode->count = 1;
    newNode->next = *head;
    *head = newNode;
}

// Inserts an n-length substring into the binary tree.
TNode* insert_substring_into_tree(TNode* root, const char* string, char followingChar) {
    if (root == NULL) {
        // Create a new node since the current position is empty.
        root = malloc(sizeof(TNode));
        if (!root) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        root->string = strdup(string);
        if (!root->string) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        root->total_count = 1;
        root->charList = NULL;
        add_char_to_list(&root->charList, followingChar);
        root->left = root->right = NULL;
    } else {
        int cmp = strcmp(string, root->string);
        if (cmp < 0) {
            // Insert into the left subtree.
            root->left = insert_substring_into_tree(root->left, string, followingChar);
        } else if (cmp > 0) {
            // Insert into the right subtree.
            root->right = insert_substring_into_tree(root->right, string, followingChar);
        } else {
            // Substring already exists; update its occurrence count and following-character list.
            root->total_count++;
            add_char_to_list(&root->charList, followingChar);
        }
    }
    return root;
}


// Processes the input file using a sliding window approach to build the binary tree.
void build_tree_from_file(FILE* inputFile, TNode** treeRoot, size_t substringLength) {
    char windowBuffer[substringLength + 1];
    size_t i;

    // Read the first 'substringLength' characters to fill the window.
    for (i = 0; i < substringLength; i++) {
        int ch = fgetc(inputFile);
        if (ch == EOF) {
            return; // The file is too short to process any substring.
        }
        windowBuffer[i] = (char)ch;
    }
    windowBuffer[substringLength] = '\0';

    int followingChar;
    // Process the file: for each new character, insert the current window and update the window.
    while ((followingChar = fgetc(inputFile)) != EOF) {
        // Insert the current n-length substring and its following character into the tree.
        *treeRoot = insert_substring_into_tree(*treeRoot, windowBuffer, (char)followingChar);
        // Slide the window: remove the first character and append the new following character.
        memmove(windowBuffer, windowBuffer + 1, substringLength - 1);
        windowBuffer[substringLength - 1] = (char)followingChar;
        windowBuffer[substringLength] = '\0';
    }
}

TNode* find_tnode(TNode *root, const char *search_string) {
    if (!root) return NULL; // If the tree or subtree is empty, return NULL

    // printf("Comparing with node: '%s' and search string: '%s'\n", root->string, search_string);
    int comparison_result = strcmp(search_string, root->string); // Compare the search string with the current node's string

    if (comparison_result < 0) {
        return find_tnode(root->left, search_string);
    } 
    else if (comparison_result > 0) {
        return find_tnode(root->right, search_string);
    } 
    else {
        return root; // If the strings are equal, return the current node
    }
}

//Print the tree, to test it
void print_tree(TNode *root) {
    if (root == NULL)
        return;
    
    print_tree(root->left); // Recursively traverse the left subtree
    
    printf("Node: '%s', Total Occurrences: %zu\n", root->string, root->total_count); // Print the current node's string and its total occurrence count

    LNode *current = root->charList; // Traverse the linked list of following characters and print each one
    while (current != NULL) {
        printf("  -> Following character: '%c', Count: %zu\n", current->character, current->count);
        current = current->next;
    }

    print_tree(root->right); // Recursively traverse the right subtree
}

//Function to get the weighted random character
char getWeightedRandomCharacter(TNode *root, const char *window_string) {
    TNode *current_node = find_tnode(root, window_string); // Find the node that corresponds to the given string

    if (current_node == NULL || current_node->total_count == 0 || !current_node->charList) {
        return '\0'; // If no node is found or the node has no valid character list, return '\0'
    }
    
    int random_value = (rand() % current_node->total_count) + 1; // Generate a random value based on the total count of characters for the given string
    //printf("Random Value: %i\n", random_value);
    // Traverse the linked list of characters to find the one corresponding to the random value
    for (LNode *current_char_node = current_node->charList; current_char_node; current_char_node = current_char_node->next) {
        random_value -= current_char_node->count;

        if (random_value <= 0) {
            //printf("\nFound character! >%c<\n", current_char_node->character);
            return current_char_node->character; // If the random value is less than or equal to the character's frequency, return that character
        }
    }
 
    return '\0'; // If no character was found (which should not happen), return '\0'
}

char* generate_text(TNode *root, int substring_length, int outputstring_length, FILE* file) {
    char start_string[substring_length + 1];
    char current_window[substring_length + 1];
    
    rewind(file);
    size_t chars_read = fread(start_string, sizeof(char), substring_length, file);
    if (chars_read == 0) {
        printf("Keine Zeichen gelesen.\n");
        return NULL;
    }
    start_string[chars_read] = '\0'; 
    //printf("Gelesene Zeichen: %zu\n", chars_read);
    //printf("Startstring: '%s'\n", start_string);
    //printf("Current window: '%s'\n", current_window);

    strcpy(current_window, start_string);
    //printf("Current window2: '%s'\n", current_window);

    char* output_string = (char*) malloc((outputstring_length + 1) * sizeof(char));
    if (output_string == NULL) {
        return NULL;
    }
    output_string[0] = '\0';

    for (int i = 0; i < outputstring_length; i++) {
        char random_character = getWeightedRandomCharacter(root, current_window);

        if (random_character == '\0') {
            printf("No valid character found for window: '%s'\n", current_window);
            break;
        }
        strncat(output_string, &random_character, 1);
        
        for (int j = 0; j < substring_length - 1; j++) {
            current_window[j] = current_window[j + 1];
        } 
        current_window[substring_length - 1] = random_character;
        current_window[substring_length] = '\0';

        //printf("Current window: '%s', Random character: '%c'\n", current_window, random_character);
    }

    output_string[outputstring_length] = '\0';

    return output_string;
}