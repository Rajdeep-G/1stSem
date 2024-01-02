#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fptr;
    fptr = fopen("input.txt", "r"); // Replace "input.txt" with your file name

    if (fptr == NULL)
    {
        printf("File not found or unable to open the file.\n");
        return 1;
    }

    int n = 6;        // Number of rows in the array, adjust as needed
    int tree[n][100]; // 2D array

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            tree[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        char line[1000] = {-1};          // Assuming a maximum line length of 1000 characters
        fgets(line, sizeof(line), fptr); // Read a line from the file

        int j = 0;
        char *token = strtok(line, " ");

        // print the token;
        printf("%s\n", token);
        
        while (token != NULL && j < 100)
        {
            tree[i][j] = atoi(token); // Convert token to integer
            token = strtok(NULL, " ");
            j++;
        }
    }

    fclose(fptr);

    // // Printing the read array for verification
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; tree[i][j] != -1; j++)
    //     {
    //         printf("%d ", tree[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}
