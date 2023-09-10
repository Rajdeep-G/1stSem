#include <stdio.h>
#include <stdlib.h>
// 2 1 4 7 5 3 6
void construct_Tree(int tree[], int index)
{
    int value;
    printf("Enter the value of the node(-1 for no node):");
    scanf("%d", &value);
    if (value == -1)
    {
        return;
    }
    tree[index] = value;
    printf("Enter the left child of %d:\n", value);
    construct_Tree(tree, 2 * index + 1);

    printf("Enter the right child of %d:\n", value);
    construct_Tree(tree, 2 * index + 2);
}

void print_inorder_traversal(int tree[],int idx){
    if(tree[idx]==-1){
        return;
    }
    print_inorder_traversal(tree,2*idx+1);
    printf("%d ",tree[idx]);
    print_inorder_traversal(tree,2*idx+2);

}

void depth_and_leaves(int tree[],int depth,int index,int *max_depth,int* number_leaves){
    if(tree[index]==-1)return;

    if (depth>*max_depth){
        *max_depth=depth;
    }

    if (tree[2*index+1]==-1 && tree[2*index+2]==-1){
        *number_leaves+=1;
    }

    depth_and_leaves(tree,depth+1,2*index+1,max_depth,number_leaves);
    depth_and_leaves(tree,depth+1,2*index+2,max_depth,number_leaves);
}
int main()
{

    printf("Constructing the tree:\n");
    int tree[100];
    for (int i = 0; i < 100; i++)
    {
        tree[i] = -1;
    }
    construct_Tree(tree, 0);

    for (int i = 0; i < 100; i++)
    {
        if (tree[i] != -1)
            printf("%d ", tree[i]);
    }

    printf("\nInorder traversal of the tree:\n");
    print_inorder_traversal(tree,0);


    int max_depth=0;
    int number_leaves=0;

    depth_and_leaves(tree,0,0,&max_depth,&number_leaves);
    printf("\n");
    printf("The maximum depth of the tree is %d\n",max_depth);
    printf("The number of leaves in the tree is %d\n",number_leaves);

}