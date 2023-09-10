#include <stdio.h>
#include <stdlib.h>

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
int main()
{
    int no_nodes;
    scanf("%d", &no_nodes);
    int useless;
    int root_value;
    scanf("%d %d", &useless, &root_value);

    struct TreeNode *root_node = create_node(root_value);
    int cost[no_nodes - 1];

    struct TreeNode *current = root_node;
    for (int i = 1; i < no_nodes; i++)
    {
        current = root_node;
        while (1)
        {
            int input;
            scanf("%d", &input);
            if (input == 0)
            {
                if (current->left == NULL)
                {
                    // printf("left\n");
                    current->left = create_node(-1);
                }
                current = current->left;
                // printf("dir\n");
            }

            else if (input == 1)
            {
                if (current->right == NULL)
                {
                    // printf("right\n");
                    current->right = create_node(-1);
                }
                current = current->right;
                // printf("dir\n");
            }

            else
            {
                current->val = input;
                // printf("got it ");
                break;
            }
        }
    }

    // print_inorder_traversal(root_node);

    int min_cost = INT_MAX;
    int no_of_nodes = 0;
    int min_path[no_nodes + 1];
    int current_path[no_nodes + 1];

    for (int i = 0; i < no_nodes + 1; i++)
    {
        min_path[i] = 0;
        current_path[i] = 0;
    }

    find_min_cost(root_node, &min_cost, 0, min_path, current_path, 0, &no_of_nodes);
    printf("\nOUTPUT OF MIN COST(of toll both) PATH\n");
    printf("%d", min_path[0]);
    for (int i = 1; i < no_of_nodes; i++)
        printf(" -> %d ", min_path[i]);

    printf("\nMin cost: %d", min_cost);
    return 0;
}
