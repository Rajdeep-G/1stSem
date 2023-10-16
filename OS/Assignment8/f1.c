#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct Node
{
    int value; // Node identifier
    int is_landmark;
    int partition;
    struct Node *neighbors; // List of neighboring nodes
} Node;

typedef struct Graph
{
    int num_nodes;
    Node *nodes; // Array of nodes
} Graph;

// Function to read the edgelist and create the initial graph
void createInitialGraph(Graph *graph)
{
    // read the edgelist from the file "initial_graph.edgelist.txt" and create the graph
    FILE *fp = fopen("loc-brightkite_edges.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    int num_nodes = 0;
    int num_edges = 0;
    while (!feof(fp))
    {
        int src, dest;
        fscanf(fp, "%d %d", &src, &dest);
        if (src > num_nodes)
            num_nodes = src;
        if (dest > num_nodes)
            num_nodes = dest;
        num_edges++;
    }

    graph->num_nodes = num_nodes;
    graph->nodes = (Node *)malloc(sizeof(Node) * num_nodes);
    // Initialize the nodes
    for (int i = 0; i < graph->num_nodes; i++)
    {
        graph->nodes[i].value = i;
        graph->nodes[i].neighbors = NULL;
        graph->nodes[i].is_landmark = 0;
        graph->nodes[i].partition = -1;
    }

    rewind(fp);

    // Add edges
    while (!feof(fp))
    {
        int node1, node2;
        if (fscanf(fp, "%d %d", &node1, &node2) != 2)
            break;
        // Add node2 to node1's neighbors
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->value = node2;
        new_node->neighbors = graph->nodes[node1].neighbors;
        graph->nodes[node1].neighbors = new_node;
        // Add node1 to node2's neighbors
        new_node = (Node *)malloc(sizeof(Node));
        new_node->value = node1;
        new_node->neighbors = graph->nodes[node2].neighbors;
        graph->nodes[node2].neighbors = new_node;
    }
    fclose(fp);
}

void printGraph(Graph *graph)
{
    for (int i = 0; i < graph->num_nodes; i++)
    {
        printf("%d: ", graph->nodes[i].value);
        Node *curr = graph->nodes[i].neighbors;
        while (curr != NULL)
        {
            printf("%d ", curr->value);
            curr = curr->neighbors;
        }
        printf("\n");
    }
}

int is_in_array(int *array, int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == value)
            return 1;
    }
    return 0;
}

int count_neighbors(Node *node)
{
    int count = 0;
    Node *curr = node->neighbors;
    while (curr != NULL)
    {
        count++;
        curr = curr->neighbors;
    }
    return count;
}

void chooseLandmarkNodes(Graph *graph)
{
    int num_landmarks = 50;
    int num_partitions = 100;

    // Initialize an array to keep track of selected landmark indices
    int landmark_indices[num_landmarks];
    // Randomly select 50 landmark nodes
    for (int i = 0; i < num_landmarks; i++)
    {
        int landmark_index;
        do
        {
            // Randomly select a node index (make sure it's not already selected)
            landmark_index = rand() % graph->num_nodes;
        } while (is_in_array(landmark_indices, i, landmark_index));
        landmark_indices[i] = landmark_index;

        // Mark this node as a landmark
        graph->nodes[landmark_index].is_landmark = 1;
    }

    // Randomly select 50 highest degree landmark nodes
    int num_highest_degree_landmarks = 50;
    int highest_degree_landmark_indices[num_highest_degree_landmarks];

    // Calculate the degrees of all nodes and store them in an array
    int node_degrees[graph->num_nodes];
    for (int i = 0; i < graph->num_nodes; i++)
    {
        node_degrees[i] = count_neighbors(&graph->nodes[i]);
    }

    //  Select 50 highest degree nodes
    for (int i = 0; i < num_highest_degree_landmarks; i++)
    {
        int max_degree = -1;
        int max_degree_index = -1;

        // Find the node with the highest degree that is not already a landmark
        for (int j = 0; j < graph->num_nodes; j++)
        {
            if (!graph->nodes[j].is_landmark && node_degrees[j] > max_degree)
            {
                max_degree = node_degrees[j];
                max_degree_index = j;
            }
        }

        highest_degree_landmark_indices[i] = max_degree_index;
        graph->nodes[max_degree_index].is_landmark = 1;
    }
    // Assign each partition to one random landmark node
    for (int i = 0; i < graph->num_nodes; i++)
    {
        if (!graph->nodes[i].is_landmark)
        {
            int landmark_index = landmark_indices[rand() % 100];
            graph->nodes[i].partition = landmark_index % num_partitions;
        }
    }
    // Write assignments to the "landmark.log" file
    FILE *landmark_log = fopen("landmark.log", "w");
    if (landmark_log == NULL)
    {
        perror("Error opening landmark.log file");
        exit(1);
    }
    // printf("%d", num_landmarks);
    for (int i = 0; i < 100; i++)
    {
        fprintf(landmark_log, "%d ", landmark_indices[i]);
        for (int j = 0; j < graph->num_nodes; j++)
        {
            if (graph->nodes[j].partition == i % num_partitions)
            {
                fprintf(landmark_log, "%d ", j);
            }
        }
        fprintf(landmark_log, "\n");
    }

    fclose(landmark_log);
}

void createRandomPairs(Graph *graph)
{
    int num_pairs = 10000;

    // Initialize random number generator
    srand(time(NULL));

    // Open the "path_to_find.log" file for writing
    FILE *path_log = fopen("path_to_find.log", "w");
    if (path_log == NULL)
    {
        perror("Error opening path_to_find.log file");
        exit(1);
    }
    for (int i = 0; i < num_pairs; i++)
    {
        // Generate two random node indices within the range of the number of nodes
        int node1 = rand() % graph->num_nodes;
        int node2 = rand() % graph->num_nodes;

        // Write the pair of node indices to the log file
        fprintf(path_log, "%d %d\n", node1, node2);
    }

    // Close the log file
    fclose(path_log);
}
// Function for graph_update threads
void *graphUpdateThread(void *arg)
{
    // while (1)
    // {
    //     // Randomly toss a coin to add or remove edges
    //     if (coin_toss(0.2))
    //     {
    //         // Add an edge
    //         // Write to "update.log"
    //     }
    //     else
    //     {
    //         // Remove an edge
    //         // Write to "update.log"
    //     }
    //     // Synchronize to avoid conflicts
    // }
}

// Function for path_finder threads
void *pathFinderThread(void *arg)
{
    while (1)
    {
        // Find paths between landmarks and partitions using Dijkstra's algorithm
        // Smartly choose nodes to find paths for
        // Synchronize to ensure safe access to the graph
    }
}

// Function for path_stitcher threads
void *pathStitcherThread(void *arg)
{
    while (1)
    {
        // Take a node pair <a, b> and find the approximate path
        // Use the latest paths found by path_finder threads
        // Remove the used edges from the graph
        // Write to "path_found.log"
        // Synchronize to avoid conflicts
    }
}

int main()
{
    // Create the graph data structure
    Graph graph;
    createInitialGraph(&graph);
    printGraph(&graph);
    // clear screen
    system("clear");

    // Choose landmark nodes and assign partitions
    chooseLandmarkNodes(&graph);

    // createRandomPairs(&graph);

    // Create thread IDs
    pthread_t graph_update_threads[5];
    pthread_t path_finder_threads[20];
    pthread_t path_stitcher_threads[10];

    // Create and initialize semaphores and locks

    // Create and launch graph_update threads
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&graph_update_threads[i], NULL, graphUpdateThread, /* pass relevant data */);
    }

    // Start the threads

    // Wait for threads to finish

    // Clean up resources (close files, destroy semaphores and locks)

    // Print the final graph in "final_graph.edgelist"

    return 0;
}
