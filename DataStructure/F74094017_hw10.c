#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#define MAX 1000

typedef struct node *treePointer;

typedef struct node
{
    char data;
    treePointer leftchild;
    treePointer rightchild;
};

int start = 0;

treePointer createNode(int data)
{
    treePointer node = (struct Node*)malloc(sizeof(struct node));
    node->data = data;
    node->leftchild = NULL;
    node->rightchild = NULL;
 
    return node;
}

treePointer createTree(int preorder[], int end, treePointer* tree)
{
    if(start > end)
    {
        return NULL;
    }

    if(preorder[start] == -1)
    {
        start++;
        return NULL;
    }

    treePointer temp = *tree;

    temp = createNode(preorder[start++]);

    temp->leftchild = createTree(preorder,end,&temp);
    temp->rightchild = createTree(preorder,end,&temp);

    return temp;
}

void swapTree(treePointer tree)
{
    if(tree == NULL)
    {
        return;
    }
    
    treePointer change = tree->leftchild;

    tree->leftchild = tree->rightchild;
    tree->rightchild = change;

    swapTree(tree->leftchild);
    swapTree(tree->rightchild);
    
}

void preorderTranversal(treePointer tree)
{
    if(tree != NULL)
    {
        printf("%d ",tree->data);

        preorderTranversal(tree->leftchild);
        preorderTranversal(tree->rightchild);
    }
}

void inorderTranversal(treePointer tree)
{
    if(tree != NULL)
    {
        inorderTranversal(tree->leftchild);

        printf("%d ",tree->data);

        inorderTranversal(tree->rightchild);
    }
}

int main()
{
    treePointer tree;
    int n, spt[MAX];

    scanf("%d",&n);

    for(int i = 0 ; i < n ; i++)
    {
        scanf("%d",&spt[i]);
    }
    
    tree = createTree(spt,n-1,&tree);

    swapTree(tree);

    preorderTranversal(tree);
    printf("\n");
    inorderTranversal(tree);

    return 0;
}