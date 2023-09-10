#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

void traverse_all_dir(int row, int col, int adj_matrix[][100], int visited[][100], int no_of_rows, int no_of_cols) // traversing all the directions
{
    visited[row][col] = 1;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0))
            {
                int new_Row = row + i;                                                                                                                               // new row
                int new_Col = col + j;                                                                                                                               // new col
                if (new_Row >= 0 && new_Row < no_of_rows && new_Col >= 0 && new_Col < no_of_cols && adj_matrix[new_Row][new_Col] == 1 && !visited[new_Row][new_Col]) // if the position is valid and not visited
                    traverse_all_dir(new_Row, new_Col, adj_matrix, visited, no_of_rows, no_of_cols);                                                                 // recursive call
            }
        }
    }
}

void countGroups(int no_of_rows, int no_of_cols, int adj_matrix[][100], int visited[][100], int *grp)
{
    for (int i = 0; i < no_of_rows; i++)
    {
        for (int j = 0; j < no_of_cols; j++)
        {
            if (adj_matrix[i][j] == 1 && !visited[i][j]) // if the position is not visited
            {
                (*grp)++;                                                            // incrementing the group count
                traverse_all_dir(i, j, adj_matrix, visited, no_of_rows, no_of_cols); // traversing all the directions
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2) // checking correctness of the command line
    {
        printf("<./a.out><inputfile.txt\n");
        exit(1);
    }
    char filename[100];
    sscanf(argv[1], "%s", filename);

    FILE *fptr = fopen(filename, "r"); // opening the file
    if (fptr == NULL)                  // cheking for file error
    {
        printf("Error! opening file");
        exit(1);
    }

    // int row, col;
    int no_of_rows, no_of_cols;
    fscanf(fptr, "%d %d", &no_of_rows, &no_of_cols);

    int adj_matrix[100][100];
    int visited[100][100];

    for (int i = 0; i < no_of_rows; i++)
    {
        for (int j = 0; j < no_of_cols; j++)
        {
            adj_matrix[i][j] = -1; // initailizing the matrix
        }
    }
    for (int i = 0; i < no_of_rows; i++)
    {
        for (int j = 0; j < no_of_cols; j++)
        {
            fscanf(fptr, "%d", &adj_matrix[i][j]); // taking input
        }
    }
    int ans = 0;
    countGroups(no_of_rows, no_of_cols, adj_matrix, visited, &ans); // counting the number of groups
    printf("%d\n", ans);
    return 0;
}