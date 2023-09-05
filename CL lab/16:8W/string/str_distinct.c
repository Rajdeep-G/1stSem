#include <stdio.h>
#include <string.h>
#define MAX_SUBSTRINGS 10000 // Adjust the size based on your needs

void my_reverse(char *str, int start_index, int end_index)
{
    while (start_index < end_index)
    {
        char temp = str[start_index];
        str[start_index++] = str[end_index];
        str[end_index--] = temp;
    }
}

// void my_copy_Substring(char *source, int start, int len, char *destination)
// {
//     for (int i = 0; i < len; i++)
//     {
//         destination[i] = source[start + i];
//     }
//     destination[len] = '\0';
// }

int isSubstringUnique(char *substring, char *uniqueSubstrings[], int uniqueCount)
{
    for (int i = 0; i < uniqueCount; i++)
    {
        if (strcmp(substring, uniqueSubstrings[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}
int isWordUnique(char *word, char *uniqueWords[], int uniqueCount)
{
    for (int i = 0; i < uniqueCount; i++)
    {
        if (strcmp(word, uniqueWords[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}


void generateAndPrintReversedSubstrings(char *input_word, int size_greater_than)
{
    int str_length = strlen(input_word);
    char *uniqueWords[MAX_SUBSTRINGS];
    int uniqueCount = 0;

    for (int len = size_greater_than; len < str_length + 1; len++)
    {
        for (int i = 0; i <= str_length - len; i++)
        {
            char word[str_length + 1];
            strcpy(word, input_word);

            my_reverse(word + i, 0, len - 1);
            if (isWordUnique(word, uniqueWords, uniqueCount))
            {
                uniqueWords[uniqueCount] = strdup(word);
                uniqueCount++;
            }
        }
    }

    // Print unique words
    for (int i = 0; i < uniqueCount; i++)
    {
        printf("%s\n", uniqueWords[i]);
        // free(uniqueWords[i]);
    }
}
int main()
{
    char input[1000];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline if present
    int size_greater_than = 2;
    generateAndPrintReversedSubstrings(input, size_greater_than);

    return 0;
}
