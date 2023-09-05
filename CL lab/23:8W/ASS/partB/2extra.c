#include <stdio.h>
#include <stdbool.h>

int adj[7][7] = {
    {0, 1, 2, 1, 0, 0, 0},
    {1, 0, 0, 0, 2, 0, 0},
    {2, 0, 0, 0, 1, 0, 4},
    {1, 0, 0, 0, 2, 1, 0},
    {0, 2, 1, 2, 0, 0, 2},
    {0, 0, 0, 1, 0, 0, 3},
    {0, 0, 4, 0, 2, 3, 0}
};

int dist[7];
bool visited[7];

int countShortestPaths(int source, int destination, int cost) {
    visited[source] = true;

    if (source == destination) {
        visited[source] = false;
        return (cost == 0) ? 1 : 0;
    }

    int count = 0;
    for (int i = 0; i < 7; i++) {
        if (adj[source][i] > 0 && !visited[i] && dist[i] + adj[source][i] == dist[source]) {
            count += countShortestPaths(i, destination, cost - adj[source][i]);
        }
    }

    visited[source] = false;
    return count;
}

int main() {
    int n = 7; // Number of vertices
    int source = 0; // First node
    int destination = n - 1; // Last node

    for (int i = 0; i < n; i++) {
        dist[i] = __INT_MAX__;
        visited[i] = false;
    }

    dist[source] = 0;

    for (int i = 0; i < n - 1; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && adj[u][v] != 0 && dist[u] + adj[u][v] <= dist[v]) {
                if (dist[u] + adj[u][v] < dist[v]) {
                    dist[v] = dist[u] + adj[u][v];
                }
            }
        }
    }

    int minCost = dist[destination];
    int count = countShortestPaths(source, destination, minCost);

    printf("Shortest distance from source to destination: %d\n", dist[destination]);
    printf("Total number of paths with the shortest distance and cost %d: %d\n", minCost, count);

    return 0;
}
