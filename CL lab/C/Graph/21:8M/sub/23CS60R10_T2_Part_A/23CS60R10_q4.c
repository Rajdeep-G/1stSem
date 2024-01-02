#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100
#define INT_MIN -1000000

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

int get_ecentricity(struct Graph *graph, int index, int num_of_vertex) // function to add ecentricity for each node
{
    int queue[num_of_vertex];
    int visited[num_of_vertex];
    int min_distance_to_each_node[num_of_vertex]; // stores the shortest distance from given node to all the nodes
    for (int i = 0; i < num_of_vertex; i++)       // initialise
    {
        queue[i] = -1;
        visited[i] = -1;
        min_distance_to_each_node[i] = 0;
    }
    int front = 0, rear = 0;
    queue[rear++] = index; // inserting the starting node
    visited[index] = 1;    // marking it as visited

    while (front < rear)
    {
        int current = queue[front++];
        struct Node *currentNode = graph->adj_list[current];
        while (currentNode != NULL) // travelling all the nodes in adj list of that particular node
        {
            if (visited[currentNode->vertex] == -1) // checking if already visited or not
            {
                queue[rear++] = currentNode->vertex;
                visited[currentNode->vertex] = 1;
                min_distance_to_each_node[currentNode->vertex] = min_distance_to_each_node[current] + 1;
            }
            currentNode = currentNode->next; // pointing to next element in the adj list
        }
    }

    int ecentricity = INT_MIN;
    for (int i = 0; i < num_of_vertex; i++) // finding the max distance from the given node to all the nodes
    {
        if (min_distance_to_each_node[i] > ecentricity)
            ecentricity = min_distance_to_each_node[i];
        // printf("%d ", min_distance_to_each_node[i]);
    }
    return ecentricity; // returning ecentricity
}

int find_diameter(struct Graph *graph, int num_of_vertex) // function to calculate diameter
{
    int diameter = INT_MIN;
    int ecentricity[num_of_vertex]; // stores the ecentricity of all the nodes
    for (int i = 0; i < num_of_vertex; i++)
        ecentricity[i] = 0; // initialiastion

    for (int i = 0; i < num_of_vertex; i++)
    {
        // printf("ecentricities are: \n");
        ecentricity[i] = get_ecentricity(graph, i, num_of_vertex); // calling the function to calculate ecentricity for each node
        // printf("\n");
    }

    for (int i = 0; i < num_of_vertex; i++) // finding the largest ecentricty cause thats called diameter
    {
        if (ecentricity[i] > diameter)
            diameter = ecentricity[i];
    }
    return diameter; // retrunng the diameter
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

    while (fgets(line, sizeof(line), fptr)) // tokenising the input from the file & storing it in the graph
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
                addEdge(graph, src, neighbor); // adding each egde
            }
            c++;
            token = strtok(NULL, " ");
        }
        // printf("\n");
    }

    // printf("Adjacency List:\n");
    // printGraph(graph);
    int diameter = find_diameter(graph, numVertices); // calling the function to calculate diameter
    printf("diameter %d\n", diameter);
    fclose(fptr);
    return 0;
}