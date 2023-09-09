#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
};

struct TreeNode *newNode(int val, struct TreeNode *parent) // create a node in the tree
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent; // marking the parent node also
    return node;
}

struct TreeNode *constructTree(int level_order[], int size) // constructing the tree from the array we have
{
    if (size == 0)
        return NULL;
    struct TreeNode *root = newNode(level_order[0], NULL); // making the root of true with the first element of the array
    // for the root node, its parent is always NULL
    int front = 0, rear = 0;
    struct TreeNode **queue = (struct TreeNode **)malloc(size * sizeof(struct TreeNode *));

    queue[rear++] = root;

    int i = 1;
    while (i < size) // continue in the loop
    {
        struct TreeNode *current = queue[front++]; // keeping track of the parent

        int left_val = level_order[i++];
        if (left_val != -1) // looking at left child
        {
            current->left = newNode(left_val, current); // forming new left child
            queue[rear++] = current->left;              // adding it to the queue
        }

        if (i >= size) // end
            break;

        int right_val = level_order[i++];
        if (right_val != -1) // looking at right child
        {
            current->right = newNode(right_val, current); // forming new right child
            queue[rear++] = current->right;               // adding it to the queue
        }
    }

    free(queue);
    return root;
}

void inorder_trav(struct TreeNode *root) // inorder traversal
{
    if (root == NULL)
        return;
    inorder_trav(root->left);
    printf("%d ", root->val);
    inorder_trav(root->right);
}

int is_in_Queue(struct TreeNode **arr, int size, struct TreeNode *node) // checking if the element (Tree node) is the queue or not (queue of tree nodes)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == node) // match found
            return 0;
    }
    return 1;
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

void find_start_Node(struct TreeNode *root, int user_Value, struct TreeNode **start_Node) // finding the start node by recursive search
{
    if (root == NULL)
        return;

    if (root->val == user_Value) // if value matches, we make that as root
    {
        *start_Node = root;
        return;
    }

    find_start_Node(root->left, user_Value, start_Node);
    find_start_Node(root->right, user_Value, start_Node);
}

int get_array_from_string(char *line, int *level_order) // a function to get the array of integers. this helps in my work
{
    int i = 0;
    char *token = strtok(line, " "); // generating the first token (separated by space)
    while (token != NULL)            // inside the loop, we generate all the remaining tokens
    {
        sscanf(token, "%d", &level_order[i]);
        i++;
        token = strtok(NULL, " ");
    }

    int j;
    for (j = 0; j < i; j++) // in this loop we check for the values in the array that are not in our need
    {
        if (level_order[j] == -2)
            level_order[j] = -1;
        continue;
        // printf("%d ", level_order[j]);
    }
    return (j - 1); // also return the final index thats alspo the length of array to deal with
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
    char line[1000];
    fgets(line, 1000, fptr); // taking the input from file
    // printf("%s", line);

    int level_order[1000]; // array to store the level order of the tree
    for (int i = 0; i < 1000; i++)
        level_order[i] = -2;                                  // initialisaion
    int len_array = get_array_from_string(line, level_order); // converting the array of char to array of int and also getting back the length of the array

    // for (int i = 0; i < 1000; i++)
    // {
    //     if (level_order[i] != -2)
    //         printf("%d ", level_order[i]);
    // }

    // int level_order[] = {1, 2, 3, -1, -1, 4, 6, -1, 5, -1, -1, 7, -1}; // -1 represents NULL
    // int size = 13;
    int size = len_array + 1;

    struct TreeNode *root = constructTree(level_order, size); // creaating the tree from the level order traversal & getting back the root
    // inorder_trav(root);
    int userValue;
    fscanf(fptr, "%d", &userValue); // taking the starting node from which we have to calculate
    fclose(fptr);

    FILE *fptr2 = fopen("output.txt", "w"); // output file
    // printf("\n%d\n", userValue);
    struct TreeNode *startNode = NULL;
    find_start_Node(root, userValue, &startNode); // we are searching & making the user given node as the startnode to start my bfs

    if (startNode)
    {
        int depth = max_depth_BFS(startNode, size); // max depth of my bfs
        printf("Minimum amount of hours needed from node %d: %d\n", userValue, depth - 1);
        fprintf(fptr2, "%d", depth - 1); // output
    }
    else
    {
        printf("Input Node is not found in the tree.\n");
        fprintf(fptr2, "Input Node is not found in the tree.\n"); // output
    }

    return 0;
}
