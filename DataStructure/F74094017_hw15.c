#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define MAX 1000

typedef struct node * treePointer;
typedef struct node
{
    int data;
    treePointer left;
    treePointer right;
};

int preIndex = 0;

treePointer postAndInConstruct(int inorder[], int postorder[], int start, int end, int *preIndex);
int pointSearch(int inorder[], int start, int end, int data);
void postorder(treePointer tree);

treePointer createNode(int data)
{
    treePointer node = (treePointer)malloc(sizeof(struct node));

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

treePointer preAndInCreate(int inorder[], int preorder[], int start, int end)
{
    if(start > end)
    {
        return NULL;
    }

    treePointer temp = createNode(preorder[preIndex++]);

    if(start == end)
    {
        return temp;
    }

    int inorderPoint = pointSearch(inorder, start, end, temp->data);

    temp->left = preAndInCreate(inorder, preorder, start, inorderPoint - 1);
    temp->right = preAndInCreate(inorder, preorder, inorderPoint + 1, end);

    return temp;
}

treePointer postAndInCreate(int inorder[], int postorder[], int end)
{
    int *pIndex = &end;//Since the V of the postorder is the last node in postorder tranversal.
    return postAndInConstruct(inorder,postorder,0,end,pIndex);
}

treePointer postAndInConstruct(int inorder[], int postorder[], int start, int end, int *preIndex)
{
    if(start > end)
    {
        return NULL;
    }

    treePointer temp = createNode(postorder[(*preIndex)--]);

    int inorderPoint = pointSearch(inorder, start, end, temp->data);

    temp->right = postAndInConstruct(inorder, postorder, inorderPoint + 1, end, preIndex);
    temp->left = postAndInConstruct(inorder, postorder, start, inorderPoint - 1, preIndex);

    return temp;
}

/**
 * @brief to find the location of the node 
 * 
 * @param inorder 
 * @param start 
 * @param end 
 * @param data 
 * @return int 
 */
int pointSearch(int inorder[], int start, int end, int data)
{
    int i;
    for(i = start ; i <= end ; i++)
    {
        if(inorder[i] == data)
        {
            return i;
        }
    }
}

void preorder(treePointer tree)
{
    if(tree != NULL)
    {
        printf("%d ",tree->data);

        preorder(tree->left);
        preorder(tree->right);
    }
}

void postorder(treePointer tree)
{
    if(tree != NULL)
    {
        postorder(tree->left);
        postorder(tree->right);

        printf("%d ",tree->data);
    }
}

int main()
{
    treePointer tree[MAX];
    int round, total, in[MAX], pre[MAX], post[MAX];
    char command[20], *printCommand[MAX];

    scanf("%d",&round);

    for(int i = 0 ; i < round ; i++)
    {
        scanf("%s",command);

        char *p = command;

        if(strcmp(p,"preorder-inorder") == 0)
        {
            preIndex = 0;
            
            scanf("%d",&total);

            for(int j = 0 ; j < total ; j++)
            {
                scanf("%d",&pre[j]);
            }
            for(int j = 0 ; j < total ; j++)
            {
                scanf("%d",&in[j]);
            }

            tree[i] = preAndInCreate(in,pre,0,total - 1);

            printCommand[i] = "postorder";
        }
        else if(strcmp(p,"postorder-inorder") == 0)
        {
            scanf("%d",&total);

            for(int j = 0 ; j < total ; j++)
            {
                scanf("%d",&post[j]);
            }
            for(int j = 0 ; j < total ; j++)
            {
                scanf("%d",&in[j]);
            }

            tree[i] = postAndInCreate(in,post,total - 1);

            printCommand[i] = "preorder";
        }
    }

    for(int i = 0 ; i < round ; i++)
    {
        if(strcmp(printCommand[i],"postorder") == 0)
        {
            postorder(tree[i]);
            printf("\n");
        }
        else if(strcmp(printCommand[i],"preorder") == 0)
        {
            preorder(tree[i]);
            printf("\n");
        }
    }
}