#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000 + 7

void mod_dfs(int current, int destination, int min_cost, int adj_mat[][100], int size, int current_cost, int *count, int visited[]) // modified dfs
{

    visited[current] = 1;

    if (current == destination) // if the destination is reached
    {
        if (current_cost == min_cost)   //  if the current cost is equal to the min cost
            (*count)++; // incrementing the count as effects in final asnwer
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (adj_mat[current][i] != 0 && !visited[i]) // if the vertex is not visited and the edge is present
                mod_dfs(i, destination, min_cost, adj_mat, size, current_cost + adj_mat[current][i], count, visited); // recursive call of dfs modified
        }
    }

    visited[current] = 0;
}


void djikstra(int adj_mat[][100], int size, int source, int shortest_distance[]) // djikstra algo
{
    int queue[size], nextnode;
    for (int i = 0; i < size; i++) // initializations
    {
        shortest_distance[i] = adj_mat[source][i];
        queue[i] = 0;
    }
    shortest_distance[source] = 0;
    queue[source] = 1;

    int count = 1;
    while (count++ < size - 1) // unless all vertices are not visited
    {
        int min_dist = INT_MAX;
        for (int i = 0; i < size; i++)
        {
            if (shortest_distance[i] < min_dist && queue[i] == 0) // if the vertex is not visited and the weight is less than the current weight
            {
                min_dist = shortest_distance[i]; // updating the min distance
                nextnode = i; // updating the next node
            }
        }
        queue[nextnode] = 1;
        for (int i = 0; i < size; i++) // updating the queue
        {
            if (queue[i] == 0) // if the vertex is not visited
            {
                if (min_dist + adj_mat[nextnode][i] < shortest_distance[i]) // if the weight is less than the current weight
                    shortest_distance[i] = min_dist + adj_mat[nextnode][i];
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

    int no_of_vertex;
    fscanf(fptr, "%d", &no_of_vertex);

    int adj_matrix[100][100];
    int new_adj_matrix[100][100];
    for (int i = 0; i < no_of_vertex; i++) // initializing the matrix
    {
        for (int j = 0; j < no_of_vertex; j++)
        {
            adj_matrix[i][j] = -1;
            new_adj_matrix[i][j] = -1;
        }
    }
    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
        {
            fscanf(fptr, "%d", &adj_matrix[i][j]); // taking input
            new_adj_matrix[i][j] = adj_matrix[i][j];
            if (adj_matrix[i][j] == 0)
                adj_matrix[i][j] = INT_MAX;
        }
    }


    int shortest_distance[100], no_of_paths[100];
    for (int i = 0; i < no_of_vertex; i++)
        shortest_distance[i] = -1;
    djikstra(adj_matrix, no_of_vertex, 0, shortest_distance); // finding the shortest distance from src to last node
    printf("%d ", shortest_distance[no_of_vertex - 1]);



// ........................................................
    int min_cost = shortest_distance[no_of_vertex - 1];
    int count = 0;
    int visited[no_of_vertex];
    for (int i = 0; i < no_of_vertex; i++)
        visited[i] = 0;
    mod_dfs(0, no_of_vertex - 1, min_cost, new_adj_matrix, no_of_vertex, 0, &count, visited); // finding the number of paths with min cost

    printf("%d", count);
    return 0;
}