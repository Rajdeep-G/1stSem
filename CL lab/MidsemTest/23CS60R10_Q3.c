#include <stdio.h>
#include <stdlib.h>

#define INF 10001

#define MAX_VERTICES 10001

typedef struct
{
    int from;
    int to;
    int weight;
} Edge;

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

void print_dfs(int adj[][100], int no_of_vertex, int start_node)
{
    int visited[no_of_vertex], stack[no_of_vertex];
    for (int i = 0; i < no_of_vertex; i++)
    {
        visited[i] = -1;
        stack[i] = -1;
    }
    int top = -1, i = 0;
    stack[++top] = start_node;
    visited[start_node] = 1;
    while (top != -1)
    {
        int current = stack[top--];
        printf("%d ", current);
        i = -1;
        while (++i < no_of_vertex)
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                stack[++top] = i;
                visited[i] = 1;
            }
        }
    }
}

void print_bfs(int adj[][100], int no_of_vertex, int start_node)
{
    int visited[no_of_vertex], queue[no_of_vertex];
    for (int i = 0; i < no_of_vertex; i++)
    {
        visited[i] = -1;
        queue[i] = -1;
    }

    int front = 0, rear = 0, i;
    queue[rear++] = start_node;
    visited[start_node] = 1;
    while (front < rear)
    {
        int current = queue[front++];
        printf("%d ", current);
        i = -1;
        while (++i < no_of_vertex)
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }

    printf("\n");
}

int max_depth_BFS(struct TreeNode *start, int size) // finding out the actual ans by using bfs
{
    if (start == NULL)
        return 0;
    // basic bfs
    struct TreeNode **queue = (struct TreeNode **)malloc(size * sizeof(struct TreeNode *));
    int front = 0, rear = 0, depth = 0; // initialisation for handling the queue

    queue[rear++] = start;
    while (front < rear)
    {
        int levelSize = rear - front;
        for (int i = 0; i < levelSize; i++)
        {
            struct TreeNode *current = queue[front++];
            if (current->parent && is_in_Queue(queue, rear, current->parent)) // adding the parent atrear end if not already present
                queue[rear++] = current->parent;
            if (current->left && is_in_Queue(queue, rear, current->left)) // adding the leftchild if not already present  at rear end
                queue[rear++] = current->left;
            if (current->right && is_in_Queue(queue, rear, current->right)) // adding the rightchild if not already present  at rear end
                queue[rear++] = current->right;
        }
        depth++; // incrementing the depth
    }
    free(queue);
    return depth; // returning the ans
}
// function to Create Edge array from given input
Edge *createEdgesArray(int n, FILE *file)
{
    Edge *edges = (Edge *)malloc(n * sizeof(Edge));
    if (edges == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        if (fscanf(file, "%d %d %d", &edges[i].from, &edges[i].to, &edges[i].weight) != 3)
        {
            fprintf(stderr, "Error reading edge information from input file.\n");
            exit(1);
        }
    }

    return edges;
}

// function to Create Distance between cities array
//  Complete the following Code
int **createDisArray(int n, int e, Edge *edges)
{
    Edge *visited[n], queue[n];
    // for (int i = 0; i < n; i++)
    // {
    //     visited[i] = -1;
    //     queue[i] = -1;
    // }

    int front = 0, rear = 0, i;
    queue[rear++] = edges[0];
    visited[0] = 1;
    while (front < rear)
    {
        int current = queue[front++];
        printf("%d ", current);
        i = -1;
        while (++i < no_of_vertex)
        {
            if (adj[current][i] == 1 && visited[i] == -1)
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
}

// Complete the following Code
// function to Find the City with the least number of neighborhood cities within Threshold
// Implement your logic to find the city with the smallest number of reachable cities
// within the threshold distance using the 'dis' array.
// Return the result here.
int findTheCity(int n, int **dis, int distanceThreshold)
{
    for (int i=0;i<n;i++){
        if (dist)     
    }
}

int main()
{
    FILE *file = fopen("input.txt", "r"); // take this as command line parameter
    if (file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    int n, e;
    fscanf(file, "%d %d", &n, &e);

    int distanceThreshold;
    fscanf(file, "%d", &distanceThreshold);

    Edge *edges = createEdgesArray(e, file);
    int **dis = createDisArray(n, e, edges);

    int result = findTheCity(n, dis, distanceThreshold);
    printf("The city with the smallest number of reachable cities within the threshold is: %d -> ", result);

    // Simply printing the output - design choice
    int flag = 0;
    for (int i = 0; i < n; i++)
    {
        if (i != result && dis[result][i] <= distanceThreshold)
        {
            if (flag == 0)
            {
                printf("%d", i);
                flag = 1;
            }
            else
            {
                printf(", %d", i);
            }
        }
    }

    // Free memory
    for (int i = 0; i < n; i++)
    {
        free(dis[i]);
    }
    free(dis);
    free(edges);
    fclose(file);
    return 0;
}