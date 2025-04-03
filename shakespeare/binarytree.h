#ifndef BINTREE
#define BINTREE

//Data structures and type definition

// Structure for the linked list that stores characters appearing after the string
typedef struct LNode {
    char character;              // Character appearing after the n-long character string
    size_t count;        // Frequency of this character
    struct LNode *next;  // Pointer to the next node in the list
} LNode;

// Structure for the binary tree node that stores n-character strings
typedef struct TNode {
    char *string;        // The n-long character string
    size_t total_count;  // Total occurrences of all following characters
    LNode *charList;     // Linked list of characters appearing after this string
    struct TNode *left;
    struct TNode *right; // Left and right child nodes in the binary tree
} TNode;

//Function declaration
FILE* openInputFile(char* str);
int isReadableFile(char* str);
void usage(char* str);
void add_char_to_list(LNode **head, char character);
TNode* insert_substring_into_tree(TNode* root, const char* substring, char followingChar);
void build_tree_from_file(FILE* inputFile, TNode** treeRoot, size_t substringLength);
TNode* find_tnode(TNode *root, const char *search_string);
void print_tree(TNode *root);
char getWeightedRandomCharacter(TNode *root, const char *window_string);
char* generate_text(TNode *root, int substring_length, int outputstring_length, FILE* file);

#endif