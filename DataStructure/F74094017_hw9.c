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
    treePointer node = (treePointer)malloc(sizeof(struct node));
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

void preorderTranversal(treePointer root)
{
    if(root == NULL)
    {
        return;
    }

    treePointer stack[MAX];
    int top = -1;

    stack[++top] = root;
    while(top != -1)
    {
        root = stack[top--];

        printf("%d ",root->data);

        if(root->rightchild != NULL)
        {
            stack[++top] = root->rightchild;
        }
        if(root->leftchild != NULL)
        {
            stack[++top] = root->leftchild;
        }
    }

}

void postorderTranversal(treePointer root)
{
    struct
    {
        treePointer node;
        bool visLeft;   
        bool visRight;  
    }stack[MAX];

    int top = -1;
    stack[++top].node = root;

    while(top != -1)
    {
        if(root->leftchild != NULL && stack[top + 1].visLeft == false)
        {
            stack[++top].visLeft = true;
            stack[top].node = root;
            root = root->leftchild;

            //search to the most left node in the tree~(if no -> go right)
            continue;
        }

        if(root->rightchild != NULL && stack[top + 1].visRight == false)
        {
            stack[++top].visRight = true;
            stack[top].node = root;
            root = root->rightchild;

            //search its left. If no, then chk right again~
            continue;
        }

        printf("%d ",root->data);

        //clean
        stack[top + 1].visLeft = false;
        stack[top + 1].visRight = false;

        root = stack[top--].node;
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

    preorderTranversal(tree);
    printf("\n");
    postorderTranversal(tree);

    return 0;
}