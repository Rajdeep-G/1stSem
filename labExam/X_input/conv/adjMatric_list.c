#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000 + 7
#define MAX_VERTICES 100

struct Node // structure for each node in the graph 
{
    int vertex;
    struct Node *next;
};

struct Graph // structure for the graph adj list
{
    struct Node *adj_list[MAX_VERTICES];
    int num_of_vertex;
};

// write a function form_adj_list(adj matrix, n) to form the adj list from the adj matrix

struct Graph *createGraph(int num_of_vertex) // creating a graph by dyn memorty
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->num_of_vertex = num_of_vertex;
    for (int i = 0; i < num_of_vertex; ++i) // initialising all the adj list to NULL
        graph->adj_list[i] = NULL;
    return graph;
}
void add_edge(struct Graph *graph, int src, int neighbours) // adding edge to the adj list
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->vertex = neighbours;
    newNode->next = graph->adj_list[src];
    graph->adj_list[src] = newNode;
}

void form_adj_list(int adj_m[][100],int size){
    struct Graph *graph = createGraph(size);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(adj_m[i][j]!=0){
                add_edge(graph,i,j);
            }
        }
    }
    for(int i=0;i<size;i++){
        printf("Adj list of %d: ",i);
        struct Node *currentNode = graph->adj_list[i];
        while(currentNode){
            printf("%d - ",currentNode->vertex);
            currentNode = currentNode->next;
        }
        printf("NULL\n");
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

    int no_of_vertex, source; // taking input
    fscanf(fptr, "%d %d", &no_of_vertex, &source);

    int adj_matrix[100][100];
    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
            adj_matrix[i][j] = -1;
    }
    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
            fscanf(fptr, "%d", &adj_matrix[i][j]); // taking adj matrix input
    }

    for (int i = 0; i < no_of_vertex; i++)
    {
        for (int j = 0; j < no_of_vertex; j++)
            printf("%d ", adj_matrix[i][j]);

        printf("\n");
    }

    form_adj_list(adj_matrix, no_of_vertex); // forming the adj list
    return 0;
}