#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int visited[1000];

int *print_dfs(int adj[][100], int no_of_vertex, int start_node)
{
    int stack[no_of_vertex];
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
        // printf("%d ", current);
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
    return visited;
}

int check_connected(int adj[][100], int no_of_vertex)
{
    int *visited = print_dfs(adj, no_of_vertex, 0);
    int i = -1;
    while (++i < no_of_vertex)
    {
        if (visited[i] == -1)
            return 0;

    }
    return 1;
}

int no_connected_components(int adj[][100], int no_of_vertex)
{
    int no_of_connected_component = 0;
    int i=0;
    while(++i<no_of_vertex)
    {
        if(visited[i]==-1)
        {
            no_of_connected_component++;
            print_dfs(adj, no_of_vertex, i);
        }
    }
    
    return no_of_connected_component;
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

    int no_of_vertex;
    fscanf(fptr, "%d", &no_of_vertex);

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

    int ans = check_connected(adj_matrix, no_of_vertex);
    if (ans == 1)
    {
        printf("Graph is connected\n");
    }
    else
    {
        int k = no_connected_components(adj_matrix, no_of_vertex);
        printf("Graph is not connected\n");
        printf("Number of connected components are %d\n", k+1);
    }
}