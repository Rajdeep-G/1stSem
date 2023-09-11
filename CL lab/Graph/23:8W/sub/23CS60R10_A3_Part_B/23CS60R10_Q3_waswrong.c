#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000 + 7


int minKey(int key[], int mstSet[], int V)
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == 0 && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

int primMST(int graph[][100], int V)
{

	int parent[V],key[V],mstSet[V];


	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = 0;

	key[0] = 0;
    parent[0] = -1;

	
	for (int count = 0; count < V - 1; count++) {
		
		int u = minKey(key, mstSet, V);
		mstSet[u] = 1;

		for (int v = 0; v < V; v++)

			if (graph[u][v] && mstSet[v] == 0
				&& graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
	}


    int mst=0;
    for(int i=1;i<V;i++)
        mst+=graph[i][parent[i]];
    
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

    int no_of_vertex, source; // taking input
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
            fscanf(fptr, "%d", &adj_matrix[i][j]); // taking adj matrix input
        }
    }


    int mst=primMST(adj_matrix, no_of_vertex);
    int per_meter_cost = 5000;
    printf("total minimum cost of constructing roads: %d\n", mst * per_meter_cost); // printing final answer
    return 0;
}