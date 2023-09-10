#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct Graph *createGraph(int num_of_vertex) // creating a graph by dyn memorty
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->num_of_vertex = num_of_vertex;
    for (int i = 0; i < num_of_vertex; ++i) // initialising all the adj list to NULL
        graph->adj_list[i] = NULL;
    return graph;
}

void addEdge(struct Graph *graph, int src, int neighbours) // adding edge to the adj list
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->vertex = neighbours;
    newNode->next = graph->adj_list[src];
    graph->adj_list[src] = newNode;
}

void form_adj_mat(struct Graph *graph, int adj_mat[100][100]) // forming the adj matrix
{
    for (int i = 0; i < graph->num_of_vertex; ++i)
    {
        struct Node *temp = graph->adj_list[i];
        while (temp)
        {
            adj_mat[i][temp->vertex] = 1;
            temp = temp->next;
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
    char file[100];
    sscanf(argv[1], "%s", file);

    FILE *fptr = fopen(file, "r"); // opening the file
    if (fptr == NULL)              // cheking for file error
    {
        printf("Error! opening file");
        exit(1);
    }

    char line[100];
    int numVertices;

    fscanf(fptr, "%d", &numVertices); // reading the number of vertices
    // printf("numVertices %d\n", numVertices);

    // fseek(fptr, 0, SEEK_SET);

    struct Graph *graph = createGraph(numVertices); // creating the graph

    while (fgets(line, sizeof(line), fptr))
    {
        int neighbor;
        char *token = strtok(line, " ");
        int src = atoi(token);
        int c = 0;
        while (token != NULL)
        {
            if (sscanf(token, "%d", &neighbor) == 1 && c != 0)
            {
                // printf("token %d ", src);
                // printf("ne %d ", neighbor);
                addEdge(graph, src, neighbor); // adding the edge
            }
            c++;
            token = strtok(NULL, " ");
        }
        // printf("\n");
    }

    int adj_mat[100][100];
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
            adj_mat[i][j] = 0;
    }
    form_adj_mat(graph, adj_mat); // forming the adj matrix

    for(int i=0;i<graph->num_of_vertex;++i)
    {
        for(int j=0;j<graph->num_of_vertex;++j)
            printf("%d ",adj_mat[i][j]);
        printf("\n");
    }
    return 0;
}