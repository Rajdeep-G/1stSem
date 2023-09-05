#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000 + 7

int min_queue(int *queue, int *visited, int size)
{
    int min = INT_MAX;
    int min_index = -1;
    int i = 0;
    while (i < size)
    {
        if (visited[i] == 0 && queue[i] < min)
        {
            min = queue[i];
            min_index = i;
        }
        i++;
    }
    return min_index;
}

int MST_prims(int adj[][100], int size, int source)
{
    int visited[size];
    int queue[size];
    int predecessor[size];
    for (int i = 0; i < size; i++)
    {
        visited[i] = 0;
        queue[i] = INT_MAX;
        predecessor[i] = -1;
    }
    queue[source] = 0;
    int count = 0;
    while (count < size)
    {
        int u = min_queue(queue, visited, size);
        visited[u] = 1;
        for (int v = 0; v < size; v++)
        {
            if (visited[v] == 0 && adj[u][v] < queue[v])
            {
                queue[v] = adj[u][v];
                predecessor[v] = u;
            }
        }
        count++;
    }
    int mst = 0;

    for (int i = 0; i < size; i++)
        mst += adj[i][predecessor[i]];

    return mst;
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

    int no_of_vertex, source;
    fscanf(fptr, "%d %d", &no_of_vertex, &source);

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

    // for (int i=0;i<no_of_vertex;i++)
    // {
    //     for (int j=0;j<no_of_vertex;j++)
    //     {
    //         printf("%d ",adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
    int per_meter_cost = 5000;
    int mst = MST_prims(adj_matrix, no_of_vertex, source);
    printf("total minimum cost of constructing roads: %d\n", mst * per_meter_cost);
    return 0;
}