#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 1000000000
struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *create_node(int value)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->val = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_inorder_traversal(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    print_inorder_traversal(root->left);
    printf("%d ", root->val);
    print_inorder_traversal(root->right);
}

void find_min_cost(struct TreeNode *root, int *min_cost, int cur_cost, int *min_path, int *current_path, int size, int *min_node_count)
{
    if (root == NULL)
        return;

    cur_cost += root->val;
    current_path[size] = root->val;

    if (root->left == NULL && root->right == NULL)
    {
        if (cur_cost < *min_cost)
        {
            *min_cost = cur_cost;
            *min_node_count = size + 1;
            for (int i = 0; i <= size; i++)
                min_path[i] = current_path[i];
        }
    }
    find_min_cost(root->left, min_cost, cur_cost, min_path, current_path, size + 1, min_node_count);
    find_min_cost(root->right, min_cost, cur_cost, min_path, current_path, size + 1, min_node_count);

    cur_cost -= root->val;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: <program name> <input_file>");
        return 0;
    }
    FILE *fptr = fopen("input.txt", "r");

    if (fptr == NULL)
    {
        printf("Error: Cannot open input file.");
        return 0;
    }

    int no_nodes;
    // scanf("%d", &no_nodes);
    fscanf(fptr, "%d", &no_nodes);
    no_nodes++;
    int tree[no_nodes][100];

    for (int i = 0; i < no_nodes; i++)
    {
        for (int j = 0; j < 100; j++)
            tree[i][j] = -1;
        
    }
    
    for (int i = 0; i < no_nodes; i++)
    {
        char line[1000];                 
        fgets(line, sizeof(line), fptr); 

        int j = 0;
        char *token = strtok(line, " ");
        while (token != NULL && j < 100)
        {
            tree[i][j] = atoi(token); 
            token = strtok(NULL, " ");
            j++;
        }
    }

    // printf("READ\n");
    int root_value = tree[1][1];
    struct TreeNode *root_node = create_node(root_value);
    struct TreeNode *current = root_node;

    // printf("TREE %d\n",root_value);
    // for (int i = 2; i < no_nodes; i++)
    // {
    //     for (int j = 0; j < 100; j++)
    //     {
    //         if (tree[i][j] != -1)
    //             printf("%d ", tree[i][j]);
    //     }
    //     // printf("\n");
    // }


    for (int i = 2; i < no_nodes; i++)
    {
        int k = 0;
        int count = 0;
        while (tree[i][k] != -1)
        {
            count++;
            k++;
        }
        // printf("%d\n", count);
        current = root_node;
        for (int j = 0; j < count - 1; j++)
        {   
            // printf("x %d ", tree[i][j]);
            if (tree[i][j] == 0)
            {
                if (current->left == NULL)
                {
                    // printf("left\n");
                    current->left = create_node(-1);
                }
                current = current->left;
                // printf("dir\n");
            }
            else if (tree[i][j] == 1)
            {
                if (current->right == NULL)
                {
                    // printf("right\n");
                    current->right = create_node(-1);
                }
                current = current->right;
                // printf("dir\n");
            }
        }
        // printf("\n");
        current->val = tree[i][count - 1];
    }

    print_inorder_traversal(root_node);
    // 5 2 6 1 3

    int min_cost = INT_MAX;
    int no_of_nodes = 0;
    int min_path[no_nodes + 1];
    int current_path[no_nodes + 1];

    for (int i = 0; i < no_nodes + 1; i++)
    {
        min_path[i] = 0;
        current_path[i] = 0;
    }
    fclose(fptr);
    FILE *fptr2 = fopen("output.txt", "w");
    find_min_cost(root_node, &min_cost, 0, min_path, current_path, 0, &no_of_nodes);
    // printf("OUTPUT OF MIN COST(of toll both) PATH\n");
    fprintf(fptr2, "OUTPUT OF MIN COST(of toll both) PATH\n");

    // printf("%d", min_path[0]);
    fprintf(fptr2, "%d", min_path[0]);
    for (int i = 1; i < no_of_nodes; i++)
    {
        // printf(" -> %d ", min_path[i]);
        fprintf(fptr2, " -> %d ", min_path[i]);
    }
    // printf("\nMinimum cost: %d", min_cost);
    fprintf(fptr2, "\nMinimum cost: %d\n", min_cost);
    return 0;
}