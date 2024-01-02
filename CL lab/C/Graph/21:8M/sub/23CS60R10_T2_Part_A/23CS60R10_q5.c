#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int visited[1000];

int modified_bfs(int adj[][100], int no_of_vertex, int start_node, int k)
{
    int visited[no_of_vertex], queue[no_of_vertex]; // same as bfs
    for (int i = 0; i < no_of_vertex; i++)
    {
        visited[i] = -1;
        queue[i] = -1;
    }

    int front = 0, rear = 0, i; // same as bfs
    queue[rear++] = start_node;
    visited[start_node] = 1;
    int cur_distance = 0;
    while (front < rear) // modified bfs
    {
        int flag = 0; // checking if all nodes are visited. if all nodes are visited and dis!=k, we can say no nodes exist at k distance.
        for (int m = 0; m < no_of_vertex; m++)
        {
            if (visited[m] == -1)
            {
                flag = 1;
                break;
            }
            if (flag == 0)
                return -1; // retruning this if all nodes are visited and dis!=k otherwise gives wrong ans.
        }
        
        int current = queue[front++];
        if (cur_distance == k)
            return (rear - front + 1); // returning the no of nodes at k distance

        i = -1;
        while (++i < no_of_vertex) // same as bfs
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
        cur_distance++;
    }

    return -1;
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

    int no_of_vertex, starting_vertex, distance_k;
    fscanf(fptr, "%d %d %d", &no_of_vertex, &starting_vertex, &distance_k);

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
    int ans = modified_bfs(adj_matrix, no_of_vertex, starting_vertex, distance_k);
    if (ans == -1)
        printf("No node at distance %d\n", distance_k);
    else
        printf("No of nodes at distance %d is %d\n", distance_k, ans);
    return 0;
}