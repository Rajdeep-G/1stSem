#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

void traverse_all_dir(int row, int col, int adj_matrix[][100], int visited[][100], int no_of_rows, int no_of_cols)
{
    visited[row][col] = 1;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0))
            {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < no_of_rows && newCol >= 0 && newCol < no_of_cols && adj_matrix[newRow][newCol] == 1 && !visited[newRow][newCol])
                    traverse_all_dir(newRow, newCol, adj_matrix, visited, no_of_rows, no_of_cols);
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
            if (adj_matrix[i][j] == 1 && !visited[i][j])
            {
                (*grp)++;
                traverse_all_dir(i, j, adj_matrix, visited, no_of_rows, no_of_cols);
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
            adj_matrix[i][j] = -1;
        }
    }
    for (int i = 0; i < no_of_rows; i++)
    {
        for (int j = 0; j < no_of_cols; j++)
        {
            fscanf(fptr, "%d", &adj_matrix[i][j]);
        }
    }
    int ans = 0;
    countGroups(no_of_rows, no_of_cols, adj_matrix, visited, &ans);
    printf("%d\n", ans);
    return 0;
}