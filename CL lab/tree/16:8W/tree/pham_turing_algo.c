#include <stdio.h>
#include <stdlib.h>

// Definition for a binary tree node
struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Function to create a new node
struct TreeNode *newNode(int val)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to construct a binary tree from level order traversal
struct TreeNode *constructTree(int level_order[], int size)
{
    if (size == 0)
    {
        return NULL;
    }

    struct TreeNode **queue = (struct TreeNode **)malloc(size * sizeof(struct TreeNode *));
    int front = 0, rear = 0;

    struct TreeNode *root = newNode(level_order[0]);
    queue[rear++] = root;

    int i = 1;
    while (i < size)
    {
        struct TreeNode *current = queue[front++];

        int left_val = level_order[i++];
        if (left_val != -1)
        {
            current->left = newNode(left_val);
            queue[rear++] = current->left;
        }

        if (i < size)
        {
            int right_val = level_order[i++];
            if (right_val != -1)
            {
                current->right = newNode(right_val);
                queue[rear++] = current->right;
            }
        }
    }

    free(queue);
    return root;
}

// Function to print the inorder traversal of the constructed tree
void inorderTraversal(struct TreeNode *root)
{
    if (root)
    {
        inorderTraversal(root->left);
        printf("%d ", root->val);
        inorderTraversal(root->right);
    }
}

int main()
{
    int level_order[] = {1, 2, 3, -1, -1, 4, 6, -1, 5, -1, -1, 7, -1}; // -1 represents NULL
    int size = sizeof(level_order) / sizeof(level_order[0]);

    struct TreeNode *root = constructTree(level_order, size);
    printf("Inorder traversal of the constructed tree: ");
    inorderTraversal(root);

    return 0;
}
