#include <stdio.h>
#include <stdlib.h>

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

struct TreeNode *construct_Tree()
{
    int value;
    printf("Enter the value of the node(-1 for no node):");
    scanf("%d", &value);
    if (value == -1)
    {
        return NULL;
    }
    struct TreeNode *newNode = create_node(value);
    printf("Enter the left child of %d:\n", value);
    newNode->left = construct_Tree();
    printf("Enter the right child of %d:\n", value);
    newNode->right = construct_Tree();
    return newNode;
}

void depth_and_leaves(struct TreeNode *root, int depth, int *max_depth, int *number_leaves)
{
    if (root == NULL)
    {
        return;
    }
    if (depth > *max_depth)
    {
        *max_depth = depth;
    }
    if (root->left == NULL && root->right == NULL)
    {
        *number_leaves += 1;
    }

    depth_and_leaves(root->left, depth + 1, max_depth, number_leaves);
    depth_and_leaves(root->right, depth + 1, max_depth, number_leaves);
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

void printCurrentLevel(struct TreeNode *root, int present_level)
{
    if (root == NULL)
        return;
    if (present_level == 0)
        printf("%d ", root->val);
    else if (present_level > 0)
    {
        printCurrentLevel(root->left, present_level - 1);
        printCurrentLevel(root->right, present_level - 1);
    }
}

void print_level_order(struct TreeNode *root, int height)
{
    int current_level = 0;
    while (current_level < height)
    {
        printCurrentLevel(root, current_level);
        printf("\n");
        current_level++;
    }
}

int main()
{
    printf("Constructing the tree:\n");
    struct TreeNode *root = construct_Tree();

    int max_depth = 0;
    int number_leaves = 0;

    depth_and_leaves(root, 0, &max_depth, &number_leaves);

    printf("The maximum depth of the tree is %d\n", max_depth);
    printf("The number of leaves in the tree is %d\n", number_leaves);

    printf("\nThe inorder traversal of the tree is:\n");
    print_inorder_traversal(root);

    printf("\nThe levelorder traversal of the tree is:\n");
    print_level_order(root, max_depth + 1);
    return 0;
}