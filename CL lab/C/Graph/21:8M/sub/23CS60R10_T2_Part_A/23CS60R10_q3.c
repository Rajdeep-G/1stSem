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

// void printGraph(struct Graph *graph)
// {
//     for (int i = 0; i < graph->num_of_vertex; ++i)
//     {
//         printf("Adj  list of  %d: ", i);
//         struct Node *currentNode = graph->adj_list[i];
//         while (currentNode)
//         {
//             printf("%d - ", currentNode->vertex);
//             currentNode = currentNode->next;
//         }
//         printf("NULL\n");
//     }
// }

int check_bipartite(struct Graph *graph, int num_of_vertex)
{
    char color[num_of_vertex]; // stores the color of each node
    int queue[num_of_vertex]; // to keep track of each node
    for (int i = 0; i < num_of_vertex; i++) // initialisation
    {
        color[i] = 'N'; // color not assigned. N for none  . two colos are B and R
        queue[i] = -1;
    }

    int front = 0, rear = 0;
    queue[rear++] = 0; 
    color[0] = 'R'; // marking color of first node
    while (front < rear)
    {
        int current = queue[front++];
        struct Node *currentNode = graph->adj_list[current];// traversing through the adj list of current node
        while (currentNode != NULL)
        {
            if (color[currentNode->vertex] == color[current]) // if color is same then not bipartite and return
                return 0;

            if (color[currentNode->vertex] == 'N') // if not visited yet
            {
                if (color[current] == 'R')
                    color[currentNode->vertex] = 'B'; // marking the color of the node 
                else
                    color[currentNode->vertex] = 'R'; // marking the color of the node
                queue[rear++] = currentNode->vertex; // adding the node to the queue
            }
            currentNode = currentNode->next;
        }
    }
    return 1; // if all the nodes are visited and no same color is found then return as bipartite
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

    // printf("Adjacency List:\n");
    // printGraph(graph);

    int ans = check_bipartite(graph, numVertices); // checking if the graph is bipartite
    fclose(fptr);
    if (ans == 1) // printing the result
        printf("Bipartite\n");
    else
        printf("Not Bipartite\n");
    return 0;
}