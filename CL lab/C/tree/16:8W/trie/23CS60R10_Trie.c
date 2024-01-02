#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

struct TrieNode // structure of the Trie
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

struct TrieNode *createNode() // creating a new node of the Trie
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode)); //dynamic memory allocation
    if (node != NULL)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            node->children[i] = NULL;
        node->isEndOfWord = 0;
    }
    return node;
}

void my_reverse(char *str, int start_index, int end_index) // reverse a string 
{
    while (start_index < end_index)
    {
        char temp = str[start_index];
        str[start_index++] = str[end_index];
        str[end_index--] = temp;
    }
}

void my_copy_Substring(const char *source, int start, int len, char *destination) // copy a string from source to destination
{
    for (int i = 0; i < len; i++)
    {
        destination[i] = source[start + i];
    }
    destination[len] = '\0';
}

void insert_trie(struct TrieNode *root, char *word) // insert into a trie
{
    struct TrieNode *node = root;
    int i = 0;
    while (word[i] != '\0') // continuing the loop till the end of the word or string
    {
        int rel_index = word[i++] - 'a'; // finding the pos of the letter to put in the corresponding part of the array
        if (node->children[rel_index] == NULL)
            node->children[rel_index] = createNode();
        node = node->children[rel_index]; // updating the pointer
    }
    node->isEndOfWord = 1; // marking the end of the word in the trie 
}

void traverse_trie(struct TrieNode *root, char *temp, int depth)
{
    if (root->isEndOfWord) // if we reach the end of a word stored in a trie, we print that 
    {
        temp[depth] = '\0';
        printf("%s\n", temp);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    {
        if (root->children[i])
        {
            temp[depth] = i + 'a';
            traverse_trie(root->children[i], temp, depth + 1); // recursively calling the trie function to reach the end
        }
    }
}

int search_trie(struct TrieNode *root,  char *word)
{
    struct TrieNode *node = root;
    int i = 0;
    while (word[i] != '\0') // continuing the loop till the end of the word or string
    {
        int rel_index = word[i++] - 'a';
        if (node->children[rel_index] == NULL)
            return 0;
        node = node->children[rel_index]; // updating the pointer
    }
    return (node != NULL && node->isEndOfWord); // returning if the word is present or not in the trie
}

void generateAndPrintReversedSubstrings(char *input_word, int size_greater_than)
{
    int str_length = strlen(input_word);
    struct TrieNode *root = createNode();
    int unique_count = 0; //keeping count of unique substrings
    int total_count = 0;
    FILE *fptr = fopen("output.txt", "w"); // output file

    for (int len = size_greater_than; len < str_length + 1; len++)
    {
        for (int i = 0; i <= str_length - len; i++) // making sure to get all the substrings
        {
            char substring[100];
            // my_copy_Substring(input_word, i, len, substring);
            strcpy(substring, input_word);
            my_reverse(substring, i, len + i - 1); // reversing the substring 
            total_count++; // increasing the toital qs
            if (!search_trie(root, substring)) // if not present in the tree 
            {
                insert_trie(root, substring); // we insert into the trie
                // printf("%s\n", substring);
                fprintf(fptr, "%s\n", substring); // writing the output into the file
                unique_count++; // increasing the count of unique string 
            }
        }
    }
    // printf("\nNo of unique stings: %d", total_count);
    // printf("\nNo of unique stings: %d", unique_count);
    fprintf(fptr, "\nNo of unique stings: %d", unique_count); // writing the total no of unqiue strings
    // traverse_trie(root, input_word, 0);

    free(root);
}

int main()
{
    char input[1000];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin); // taking input from the user 
    input[strcspn(input, "\n")] = '\0';
    int size_greater_than = 2; // specified that substrings are of length >=2 

    for (int i = 0; i < strlen(input); i++) // converting into lower case if any letter is in uppercase
        input[i] = tolower(input[i]); 

    generateAndPrintReversedSubstrings(input, size_greater_than); // calling the actual working function

    return 0;
}
