#include <stdio.h>
#include <string.h>

void my_reverse(char *str, int start_index, int end_index)
{
    while (start_index < end_index)
    {
        char temp = str[start_index];
        str[start_index++] = str[end_index];
        str[end_index--] = temp;
    }
}

void my_copy_Substring(char *source, int start, int len, char *destination)
{
    for (int i = 0; i < len; i++)
    {
        destination[i] = source[start + i];
    }
    destination[len] = '\0';
}

void generateAndPrintReversedSubstrings(char *input_word, int size_greater_than)
{
    int str_length = strlen(input_word);

    for (int len = size_greater_than; len < str_length + 1; len++)
    {
        for (int i = 0; i <= str_length - len; i++)
        {
            char substring[100];
            // my_copy_Substring(input_word, i, len, substring);
            strcpy(substring, input_word);

            my_reverse(substring, i, len + i - 1);
            printf("%s\n", substring);
        }
        
    }
}

int main()
{
    char input[1000];
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    int size_greater_than = 2;
    generateAndPrintReversedSubstrings(input, size_greater_than);

    return 0;
}
