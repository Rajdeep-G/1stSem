#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_dfs(int adj[][100], int no_of_vertex, int start_node)
{
    int visited[no_of_vertex], stack[no_of_vertex];
    for (int i = 0; i < no_of_vertex; i++)
    {
        visited[i] = -1;
        stack[i] = -1;
    }
    int top = -1, i = 0;
    stack[++top] = start_node;
    visited[start_node] = 1;
    while (top != -1)
    {
        int current = stack[top--];
        printf("%d ", current);
        i = -1;
        while (++i < no_of_vertex)
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                stack[++top] = i;
                visited[i] = 1;
            }
        }
    }
}

void print_bfs(int adj[][100], int no_of_vertex, int start_node)
{
    int visited[no_of_vertex], queue[no_of_vertex];
    for (int i = 0; i < no_of_vertex; i++)
    {
        visited[i] = -1;
        queue[i] = -1;
    }

    int front = 0, rear = 0, i;
    queue[rear++] = start_node;
    visited[start_node] = 1;
    while (front < rear)
    {
        int current = queue[front++];
        printf("%d ", current);
        i = -1;
        while (++i < no_of_vertex)
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }

    printf("\n");
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

    int no_of_vertex, starting_vertex;
    fscanf(fptr, "%d %d", &no_of_vertex, &starting_vertex);

    int adj_matrix[100][100];
    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
        {
            adj_matrix[i][j] = -1;
        }
    }
    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
        {
            fscanf(fptr, "%d", &adj_matrix[i][j]);
        }
    }

    printf("BFS: ");
    print_bfs(adj_matrix, no_of_vertex, starting_vertex);
    printf("\nDFS: ");
    print_dfs(adj_matrix, no_of_vertex, starting_vertex);
}