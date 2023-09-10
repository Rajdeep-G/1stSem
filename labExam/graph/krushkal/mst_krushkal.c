#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000 + 7

// Structure to represent an edge in the graph
struct Edge
{
    int src, dest, weight;
};

// Structure to represent a disjoint-set (used for cycle detection)
struct DisjointSet
{
    int *parent, *rank;
    int size;
};

// Function to create a new DisjointSet with 'size' elements
struct DisjointSet *createDisjointSet(int size)
{
    struct DisjointSet *ds = (struct DisjointSet *)malloc(sizeof(struct DisjointSet));
    ds->size = size;
    ds->parent = (int *)malloc(size * sizeof(int));
    ds->rank = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i)
    {
        ds->parent[i] = i;
        ds->rank[i] = 0;
    }
    return ds;
}

// Function to find the set (representative) of an element using path compression
int find(struct DisjointSet *ds, int i)
{
    if (i != ds->parent[i])
        ds->parent[i] = find(ds, ds->parent[i]); // Path compression
    return ds->parent[i];
}

// Function to perform union of two sets by rank
void unionSets(struct DisjointSet *ds, int x, int y)
{
    int rootX = find(ds, x);
    int rootY = find(ds, y);

    if (ds->rank[rootX] < ds->rank[rootY])
        ds->parent[rootX] = rootY;
    else if (ds->rank[rootX] > ds->rank[rootY])
        ds->parent[rootY] = rootX;
    else
    {
        ds->parent[rootY] = rootX;
        ds->rank[rootX]++;
    }
}

// Comparator function for sorting edges by weight
int compareEdges(const void *a, const void *b)
{
    return ((struct Edge *)a)->weight - ((struct Edge *)b)->weight;
}

// Function to find the MST using Kruskal's algorithm
void KruskalMST(int **adjMatrix, int V)
{
    int numEdges = 0;
    struct Edge *result = (struct Edge *)malloc((V - 1) * sizeof(struct Edge));
    struct DisjointSet *ds = createDisjointSet(V);

    // Create an array of all edges from the adjacency matrix
    struct Edge *edges = (struct Edge *)malloc(V * V * sizeof(struct Edge));
    for (int i = 0; i < V; ++i)
    {
        for (int j = i + 1; j < V; ++j)
        {
            if (adjMatrix[i][j] != 0)
            {
                edges[numEdges].src = i;
                edges[numEdges].dest = j;
                edges[numEdges].weight = adjMatrix[i][j];
                numEdges++;
            }
        }
    }

    // Sort the edges by weight
    qsort(edges, numEdges, sizeof(struct Edge), compareEdges);

    int mstWeight = 0;
    for (int i = 0; i < numEdges && result != NULL; ++i)
    {
        int x = find(ds, edges[i].src);
        int y = find(ds, edges[i].dest);

        if (x != y)
        {
            result[mstWeight] = edges[i];
            mstWeight++;
            unionSets(ds, x, y);
        }
    }
    if (mstWeight < V - 1)
    {
        printf("No MST exists for the given graph.\n");
        return;
    }

    printf("Edges in the Minimum Spanning Tree:\n");
    for (int i = 0; i < V - 1; ++i)
    {
        printf("Edge %d-%d: Weight %d\n", result[i].src, result[i].dest, result[i].weight);
    }

    // Free allocated memory
    free(result);
    free(ds->parent);
    free(ds->rank);
    free(ds);
    free(edges);
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

    int no_of_vertex; // taking input
    fscanf(fptr, "%d", &no_of_vertex);

    int **adj_matrix = (int **)malloc(no_of_vertex * sizeof(int *)); // allocating memory for adj matrix
    for (int i = 0; i < no_of_vertex; i++)
    {
        adj_matrix[i] = (int *)malloc(no_of_vertex * sizeof(int));
    }

    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
            fscanf(fptr, "%d", &adj_matrix[i][j]); // taking adj matrix input
    }

    KruskalMST(adj_matrix, no_of_vertex);

    // printf("%d\n", val);
    for (int i = 0; i < no_of_vertex; i++)
    {
        free(adj_matrix[i]);
    }
    free(adj_matrix);

    return 0;
}