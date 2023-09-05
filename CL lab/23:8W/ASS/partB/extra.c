#include <stdio.h>
#include <stdbool.h>

int adj[7][7] = {
    {0, 1, 2, 1, 0, 0, 0},
    {1, 0, 0, 0, 2, 0, 0},
    {2, 0, 0, 0, 1, 0, 4},
    {1, 0, 0, 0, 2, 1, 0},
    {0, 2, 1, 2, 0, 0, 2},
    {0, 0, 0, 1, 0, 0, 3},
    {0, 0, 4, 0, 2, 3, 0}};

int dist[7];
bool visited[7];
int totalPaths = 0;

void DFS(int s, int d, int n, int *index, int *path)
{
    path[(*index)++] = s;
    visited[s] = true;

    if (s == d)
    {
        for (int i = 0; i < *index; i++)
            printf("%d ", path[i]);

        printf("\n");
        totalPaths++;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (adj[s][i] != 0 && !visited[i])
                DFS(i, d, n, index, path);
        }
    }
    (*index)--;
    visited[s] = false;
}

int findMinDist(int n)
{
    int minDist = -1;
    int minIndex = -1;

    for (int i = 0; i < n; i++)
    {
        if (!visited[i] && (minDist == -1 || dist[i] < minDist))
        {
            minDist = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void dijkstra(int source, int n)
{
    for (int i = 0; i < n; i++)
    {
        dist[i] = __INT_MAX__;
        visited[i] = false;
    }

    dist[source] = 0;

    for (int i = 0; i < n - 1; i++)
    {
        int u = findMinDist(n);
        visited[u] = true;

        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && adj[u][v] != 0 && dist[u] + adj[u][v] < dist[v])
            {
                dist[v] = dist[u] + adj[u][v];
            }
        }
    }
}

int main()
{
    int n = 7;               // Number of vertices
    int source = 0;          // First node
    int destination = n - 1; // Last node

    dijkstra(source, n);

    printf("Shortest distance from source to destination: %d\n", dist[destination]);

    int index = 0;
    int path[10];
    int s = source;
    int d = destination;

    for (int i = 0; i < 7; i++)
    {
        visited[i] = false;
    }

    printf("Paths from source to destination:\n");
    DFS(s, d, n, &index, path);

    printf("Total number of paths: %d\n", totalPaths);

    return 0;
}
