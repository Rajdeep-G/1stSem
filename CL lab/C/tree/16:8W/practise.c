
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode
{
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *create_node(int val)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->value = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void preorder(struct TreeNode *root)
{
    if (root == NULL)
        return;
    printf("%d", root->value);
    preorder(root->left);
    preorder(root->right);
}

struct TreeNode *constructTree(int lo[], int size)

{
    if (size == 0)
        return NULL;

    struct TreeNode *root = create_node(lo[0]);

    struct TreeNode *queue[100];

    int i = 1;
    int f = 0;
    int r = 0;
    queue[r++] = lo[0];
    while (i < size)
    {
        struct TreeNode *cur = queue[f++];
        int left = lo[i++];
        if (left != -1)
        {
            cur->left = create_node(left);
            queue[r++] = cur->left;
        }
        if (i >= size)
            break;
        int right = lo[i++];
        if (right != -1)
        {
            cur->right = create_node(left);
            queue[r++] = cur->right;
        }
    }
}

int maxDepth(struct TreeNode *root)
{
    if (root == NULL)
        return 0;
    struct TreeNode *queue[100];
    int f = 0, r = 0;
    queue[r++]=root;
    int depth=0;
    while(f<r){
        int iter=f-r;
        for(int i=0;i<iter;i++){
            struct TreeNode * cur=queue[f++];
            if(cur->left)
            queue[r++]=cur->left;
            if(cur->right)
            queue[r++]=cur->right;
        }
        depth++;
    }
    return depth;
}