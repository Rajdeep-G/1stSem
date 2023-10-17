#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#define INT_MAX 1000000
#define gl_landmark 100
#define gl_partition 100
#define gl_high_degree_landmark 5
#define gl_random_landmark 5

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

typedef struct Partition
{
    int num_nodes;
    int *nodes;
    int landmark;
} Partition;
// Partition *partitions = (Partition *)malloc(sizeof(Partition) * num_partitions);
Partition partitions[gl_landmark];

// linked list for path
typedef struct PathNode
{
    int node;              // Node identifier in the path
    struct PathNode *next; // Pointer to the next node in the path
} PathNode;

typedef struct
{
    int distance;   // Node identifier
    PathNode *head; // Head of the linked list representing the path
} Path;
Path *landmark_paths[gl_landmark][gl_landmark];
// function definitions

void createInitialGraph(Graph *graph);
void printGraph(Graph *graph);
int is_in_array(int *array, int size, int value);
int count_neighbors(Node *node);
void write_landmark_log(int *landmark_indices, Graph *graph, int num_partitions, int num_landmarks);
void chooseLandmarkNodes(Graph *graph);
void createRandomPairs(Graph *graph);
void *graphUpdateThread(void *arg);
void add_edge(Graph *graph, int node_src, int node_dest);
void remove_edge(Graph *graph, int node_src, int node_dest);
void log_update(char *type, int node0, int node1);
void performGraphUpdate(Graph *graph);
double random_uniform();
void *pathFinderThread(void *arg);
void *pathStitcherThread(void *arg);

// Function to read the edgelist and create the initial graph
void createInitialGraph(Graph *graph)
{
    FILE *fp = fopen("loc-brightkite_edges.txt", "r");
    // FILE *fp=fopen("input.txt","r");
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
    graph->nodes = (Node *)malloc(sizeof(Node) * num_nodes); // Allocate memory for the nodes

    for (int i = 0; i < graph->num_nodes; i++)
    {
        graph->nodes[i].value = i;
        graph->nodes[i].neighbors = NULL;
        graph->nodes[i].is_landmark = 0;
        graph->nodes[i].partition = -1;
    }

    rewind(fp); // Go back to the beginning of the file

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
    int k = 0;
    while (k < size)
    {
        if (array[k] == value)
            return 1;
        k++;
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

void write_landmark_log(int *landmark_indices, Graph *graph, int num_partitions, int num_landmarks)
{

    FILE *landmark_log = fopen("landmark.log", "w");
    if (landmark_log == NULL)
    {
        perror("Error opening landmark.log file");
        exit(1);
    }

    for (int i = 0; i < num_partitions; i++)
    {
        partitions[i].nodes = (int *)malloc(sizeof(int) * graph->num_nodes);
        partitions[i].num_nodes = 0;
        partitions[i].landmark = -1;
    }
    for (int i = 0; i < num_partitions; i++)
    {
        fprintf(landmark_log, "%d ", landmark_indices[i]);
        partitions[i].landmark = landmark_indices[i];
        int j;
        int c = 0;
        for (j = 0; j < graph->num_nodes; j++)
        {
            if (graph->nodes[j].partition == i % num_partitions)
            {
                fprintf(landmark_log, "%d ", j);
                // partitions[i].nodes[j] = j;
                partitions[i].nodes[c++] = j;
            }
        }
        fprintf(landmark_log, "\n");
        partitions[i].num_nodes = c;
        // partitions->num_nodes = j;
    }

    fclose(landmark_log);

    // for (int m = 0; m < 4; m++)
    // {
    //     printf("%d ", partitions[m].landmark);
    //     printf("%d ", partitions[m].num_nodes);
    //     printf("\n");
    // }
}

void chooseLandmarkNodes(Graph *graph)
{
    int num_landmarks = gl_random_landmark;
    int num_total_landmarks = gl_landmark;
    int num_partitions = gl_partition;
    srand(time(NULL));
    int landmark_indices[num_landmarks];
    // Randomly select 50 landmark nodes
    for (int i = 0; i < num_landmarks; i++)
    {
        int landmark_index = rand() % graph->num_nodes;
        if (graph->nodes[landmark_index].is_landmark == 1)
            i--;
        else
        {
            landmark_indices[i] = landmark_index;
            graph->nodes[landmark_index].is_landmark = 1;
        }
    }

    // for (int x = 0; x < num_landmarks; x++)
    //     printf("%d ", landmark_indices[x]);

    int num_highest_degree_landmarks = gl_high_degree_landmark;
    int highest_degree_landmark_indices[num_highest_degree_landmarks];

    int node_degrees[graph->num_nodes];
    for (int i = 0; i < graph->num_nodes; i++)
        node_degrees[i] = count_neighbors(&graph->nodes[i]); // Count the number of neighbors for each node

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

    // for (int x=0;x<num_highest_degree_landmarks;x++)
    //     printf("%d ", highest_degree_landmark_indices[x]);

    // Assign each partition to one random landmark node
    for (int i = 0; i < graph->num_nodes; i++)
    {
        if (!graph->nodes[i].is_landmark)
        {
            int landmark_index = landmark_indices[rand() % gl_landmark];
            graph->nodes[i].partition = landmark_index % num_partitions;
        }
    }

    write_landmark_log(landmark_indices, graph, num_partitions, num_total_landmarks);
}

void createRandomPairs(Graph *graph)
{
    int num_pairs = 10000;
    srand(time(NULL));
    FILE *path_log = fopen("path_to_find.log", "w");
    if (path_log == NULL)
    {
        perror("Error opening path_to_find.log file");
        exit(1);
    }
    for (int i = 0; i < num_pairs; i++)
        fprintf(path_log, "%d %d\n", rand() % graph->num_nodes, rand() % graph->num_nodes);
    fclose(path_log);
}

// ############################################################################################################

// Mutex for synchronizing access to the "update.log" file
pthread_mutex_t log_mutex;
// Function to generate a random number in the range [0, 1]
double random_uniform()
{
    return (double)rand() / RAND_MAX;
}

int edge_exists(Node *node, int value)
{
    Node *curr = node->neighbors;
    while (curr != NULL)
    {
        if (curr->value == value)
            return 1;
        curr = curr->neighbors;
    }
    return 0;
}

void add_edge(Graph *graph, int node_src, int node_dest)
{
    if (edge_exists(&graph->nodes[node_src], node_dest))
        return;
    Node *new_neighbour_node0 = (Node *)malloc(sizeof(Node));
    new_neighbour_node0->value = node_dest;
    new_neighbour_node0->neighbors = graph->nodes[node_src].neighbors;
    graph->nodes[node_src].neighbors = new_neighbour_node0;

    Node *new_neighbour_node1 = (Node *)malloc(sizeof(Node));
    new_neighbour_node1->value = node_src;
    new_neighbour_node1->neighbors = graph->nodes[node_dest].neighbors;
    graph->nodes[node_dest].neighbors = new_neighbour_node1;

    // printf("FINISHED ADDING");
}

void remove_edge(Graph *graph, int node_src, int node_dest)
{
    Node *curr = graph->nodes[node_src].neighbors;
    Node *prev = NULL;
    while (curr != NULL)
    {
        if (curr->value == node_dest)
        {
            if (prev == NULL)
                graph->nodes[node_src].neighbors = curr->neighbors;
            else
                prev->neighbors = curr->neighbors;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->neighbors;
    }

    curr = graph->nodes[node_dest].neighbors;
    prev = NULL;
    while (curr != NULL)
    {
        if (curr->value == node_src)
        {
            if (prev == NULL)
                graph->nodes[node_dest].neighbors = curr->neighbors;
            else
                prev->neighbors = curr->neighbors;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->neighbors;
    }

    // printf("FINISHED REMOVING");
}

void log_update(char *type, int node0, int node1)
{
    time_t current_time;
    struct tm *time_info;
    char time_string[20];

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // pthread_mutex_lock(&log_mutex);
    FILE *update_log = fopen("update.log", "a");
    if (update_log == NULL)
    {
        perror("Error opening update.log file");
        exit(1);
    }

    fprintf(update_log, "<%s> <%d, %d> <%s>\n", type, node0, node1, time_string);

    fclose(update_log);
    // pthread_mutex_unlock(&log_mutex);
}

void performGraphUpdate(Graph *graph)
{

    static int num = 0;
    if (random_uniform() < 0.2)
    {

        int node0 = 0;
        int node1 = 0;
        while (1)
        {
            node0 = rand() % graph->num_nodes;
            node1 = rand() % graph->num_nodes;
            if (node0 != node1 && !edge_exists(&graph->nodes[node0], node1))
                break;
        }
        pthread_mutex_lock(&log_mutex);
        // printf("HI lock add\n");
        add_edge(graph, node0, node1);
        log_update("ADD", node0, node1);
        pthread_mutex_unlock(&log_mutex);
        printf("ADD %d \n", num);
    }
    else
    {
        int node2 = 0;
        int node3 = 0;
        while (1)
        {
            node2 = rand() % graph->num_nodes;
            node3 = rand() % graph->num_nodes;
            if (node2 != node3 && edge_exists(&graph->nodes[node2], node3))
                break;
        }
        pthread_mutex_lock(&log_mutex);
        // printf("HI lock remove\n");
        remove_edge(graph, node2, node3);
        log_update("REMOVE", node2, node3);
        pthread_mutex_unlock(&log_mutex);
        printf("REMOVE %d \n", num);
    }
    num++;
}

// Thread function for graph_update threads
void *graphUpdateThread(void *data)
{
    Graph *graph = (Graph *)data;
    performGraphUpdate(graph);
    return NULL;
}

// ############################################################################################################

PathNode *constructPath(int predesessor[], int src, int dest)
{
    PathNode *head = NULL;
    PathNode *curr = NULL;
    int curr_node = dest;
    while (curr_node != src)
    {
        PathNode *new_node = (PathNode *)malloc(sizeof(PathNode));
        new_node->node = curr_node;
        new_node->next = NULL;
        if (head == NULL)
        {
            head = new_node;
            curr = new_node;
        }
        else
        {
            curr->next = new_node;
            curr = new_node;
        }
        curr_node = predesessor[curr_node];
    }
    PathNode *new_node = (PathNode *)malloc(sizeof(PathNode));
    new_node->node = src;
    new_node->next = NULL;
    if (head == NULL)
    {
        head = new_node;
        curr = new_node;
    }
    else
    {
        curr->next = new_node;
        curr = new_node;
    }

    return head;
}

int minDistance_djikstra(int distances[], int visited[], int num_nodes)
{
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < num_nodes; i++)
    {
        if (visited[i] == 0 && distances[i] <= min)
        {
            min = distances[i];
            min_index = i;
        }
    }
    return min_index;
}

void djikstra_landmark(Graph *graph, Partition *partitions, int num_partitions, Path *landmark_paths[][100])
{
    printf("HI\n");
    int num_landmarks = gl_landmark;
    // for (int i = 0; i < num_landmarks; i++)
    // {
    //     for (int j = 0; j < num_landmarks; j++)
    //     {
    int i = 0;
    int j = 4;
    int src = partitions[i].landmark;
    int dest = partitions[j].landmark;

    printf("%d %d\n", src, dest);
    if (src == dest)
        return;

    int distances[graph->num_nodes];
    int visited[graph->num_nodes];
    int predesessor[graph->num_nodes];
    for (int k = 0; k < graph->num_nodes; k++)
    {
        distances[k] = INT_MAX;
        visited[k] = 0;
        predesessor[k] = -1;
    }
    distances[src] = 0;
    for (int k = 0; k < graph->num_nodes - 1; k++)
    {
        int min_index = minDistance_djikstra(distances, visited, graph->num_nodes);
        visited[min_index] = 1;
        Node *curr = &graph->nodes[min_index];
        while (curr != NULL)
        {
            int v = curr->value;
            if (!visited[v] && distances[min_index] != INT_MAX && distances[min_index] + 1 < distances[v])
            {
                distances[v] = distances[min_index] + 1;
                predesessor[v] = min_index;
            }

            curr = curr->neighbors;
        }
    }
    landmark_paths[i][j]->distance = distances[dest];
    landmark_paths[i][j]->head = constructPath(predesessor, src, dest);
    // store the path in landmark_paths[i][j]
    printf("HI\n");
}

void print_djikstra_landmark_path(int src, int dest)
{
    printf("Shortest path %d\n", landmark_paths[src][dest]->distance);
    PathNode *curr = landmark_paths[src][dest]->head;
    while (curr != NULL)
    {
        printf("%d ", curr->node);
        curr = curr->next;
    }
}

void performPathFind(Graph *graph)
{
    // node to landmark distance
    djikstra_landmark(graph, partitions, gl_landmark, landmark_paths);
    // int src = 0;
    // int dest = 4;
    // print_djikstra_landmark_path(src, dest);



    // landmark to landmark distance
}

void *pathFinderThread(void *arg)
{
    Graph *graph = (Graph *)arg;
    performPathFind(graph);
    return NULL;
}

void initialise_landmark_paths()
{

    for (int i = 0; i < gl_landmark; i++)
    {
        for (int j = 0; j < gl_landmark; j++)
        {

            landmark_paths[i][j] = (Path *)malloc(sizeof(Path));
            landmark_paths[i][j]->head = NULL;
            if (i == j)
                landmark_paths[i][j]->distance = 0;
            else
                landmark_paths[i][j]->distance = INT_MAX;
        }
    }
}

// ############################################################################################################

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

// ############################################################################################################

int main()
{

    if (pthread_mutex_init(&log_mutex, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }
    Graph graph;

    createInitialGraph(&graph);
    // printGraph(&graph);

    // system("clear");

    chooseLandmarkNodes(&graph);
    // createRandomPairs(&graph);

    pthread_t graph_update_threads[5];
    pthread_t path_finder_threads[20];

    // .........................
    int error;
    // // Create and launch graph_update threads
    for (int i = 0; i < 5; i++)
    {
        error = pthread_create(&(graph_update_threads[i]), NULL, graphUpdateThread, &graph);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
                   strerror(error));
    }

    for (int i = 0; i < 5; i++)
        pthread_join(graph_update_threads[i], NULL);
    // .........................

    // for (int i = 0; i < 20; i++)
    // {
    //     error = pthread_create(&(path_finder_threads[i]), NULL, pathFinderThread, &graph);
    //     if (error != 0)
    //         printf("\nThread can't be created :[%s]",
    //                strerror(error));
    // }

    // for (int i = 0; i < 20; i++)
    //     pthread_join(path_finder_threads[i], NULL);

    // pthread_mutex_destroy(&log_mutex);

    // .............................
    initialise_landmark_paths();
    pathFinderThread(&graph);

    return 0;
}
