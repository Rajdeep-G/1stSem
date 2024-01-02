#include <stdio.h>
#include <stdlib.h>

#define INF 10001

#define MAX_VERTICES 10001
#define INT_MAX 1000000 + 7
typedef struct
{
    int from;
    int to;
    int weight;
} Edge;

Edge *createEdgesArray(int n, FILE *file)
{
    Edge *edges = (Edge *)malloc(n * sizeof(Edge));
    if (edges == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        if (fscanf(file, "%d %d %d", &edges[i].from, &edges[i].to, &edges[i].weight) != 3)
        {
            fprintf(stderr, "Error reading edge information from input file.\n");
            exit(1);
        }
    }

    return edges;
}

// void djikstra(int adj_mat[][5], int size, int source, int shortest_distance[])
// {

//     int queue[size], nextnode;
//     for (int i = 0; i < size; i++)
//     {
//         shortest_distance[i] = adj_mat[source][i];
//         queue[i] = 0;
//     }
//     shortest_distance[source] = 0;
//     queue[source] = 1;
//     int count = 1;
//     while (count++ < size - 1)
//     {
//         int min_dist = INT_MAX;
//         for (int i = 0; i < size; i++) // basically find out the shortest distance node from the source
//         {
//             if (shortest_distance[i] < min_dist && queue[i] == 0)
//             {
//                 min_dist = shortest_distance[i];
//                 nextnode = i;
//             }
//         }
//         queue[nextnode] = 1;
//         for (int i = 0; i < size; i++)
//         {
//             if (queue[i] == 0) // relax the edges
//             {
//                 if (min_dist + adj_mat[nextnode][i] < shortest_distance[i])
//                     shortest_distance[i] = min_dist + adj_mat[nextnode][i];
//             }
//         }
//     }
// }

void dijkstra(int adj_mat[][100], int size, int source, int shortest_distance[]) {
    int visited[size], nextnode;

    // Initialize the visited array and shortest_distance array
    for (int i = 0; i < size; i++) {
        shortest_distance[i] = INT_MAX;
        visited[i] = 0;
    }

    // Distance from source to itself is 0
    shortest_distance[source] = 0;

    for (int i = 0; i < size; i++) {
        int min_dist = INT_MAX;

        // Find the node with the minimum distance from the source
        for (int j = 0; j < size; j++) {
            if (!visited[j] && shortest_distance[j] < min_dist) {
                min_dist = shortest_distance[j];
                nextnode = j;
            }
        }

        // Mark the selected node as visited
        visited[nextnode] = 1;

        // Update the distances of adjacent nodes
        for (int k = 0; k < size; k++) {
            if (!visited[k] && adj_mat[nextnode][k] != 0) {
                if (shortest_distance[nextnode] + adj_mat[nextnode][k] < shortest_distance[k]) {
                    shortest_distance[k] = shortest_distance[nextnode] + adj_mat[nextnode][k];
                }
            }
        }
    }
}
int **createDisArray(int n, int e, Edge *edges)
{
    // create a adjacent matrix of size n*n
    int **dis = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        dis[i] = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dis[i][j] = 0;
        }
    }

    for (int i = 0; i < e; i++)
        dis[edges[i].from][edges[i].to] = edges[i].weight;



    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {
    //         if (dis[i][j] == -1)
    //             dis[i][j] = INT_MAX;
    //     }
    // }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", dis[i][j]);
        }
        printf("\n");
    }

    return dis;
}

int findTheCity(int n, int **dis, int distanceThreshold)
{

    int ans[n];
    for (int i = 0; i < n; i++)
        ans[i] = 0;

    for (int i = 0; i < n; i++)
    {
        int shortest_distance[n];
        for (int j = 0; j < n; j++)
            shortest_distance[j] = -1;
        dijkstra(dis, n, i, shortest_distance);
        for (int j = 0; j < n; j++)
            printf("%d ", shortest_distance[j]);
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (shortest_distance[j] <= distanceThreshold && j != i)
                count++;
        }
        // printf("%d\n", count);
        printf("\n");
        ans[i] = count;
    }

    for (int i = 0; i < n; i++)
        printf("%d ", ans[i]);

    return 0;
}

int main()
{
    FILE *file = fopen("3_input.txt", "r"); // take this as command line parameter
    if (file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    int n, e;
    fscanf(file, "%d %d", &n, &e);

    int distanceThreshold;
    fscanf(file, "%d", &distanceThreshold);

    Edge *edges = createEdgesArray(e, file);
    int **dis = createDisArray(n, e, edges);

    int result = findTheCity(n, dis, distanceThreshold);
    // printf("The city with the smallest number of reachable cities within the threshold is: %d -> ", result);

    // // Simply printing the output - design choice
    // int flag = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     if (i != result && dis[result][i] <= distanceThreshold)
    //     {
    //         if (flag == 0)
    //         {
    //             printf("%d", i);
    //             flag = 1;
    //         }
    //         else
    //         {
    //             printf(", %d", i);
    //         }
    //     }
    // }

    // Free memory
    for (int i = 0; i < n; i++)
    {
        free(dis[i]);
    }
    free(dis);
    free(edges);
    fclose(file);
    return 0;
}