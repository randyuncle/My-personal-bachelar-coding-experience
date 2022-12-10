#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>

typedef struct threadedTree *treePointer;
typedef struct threadedTree
{
    int data;
    treePointer leftchild;
    treePointer rightchild;
    bool leftThread;
    bool rightThread;
};

treePointer createNode(int data);
void createTree(int data, char *direction, int insert, treePointer *tree);
void insertLeft(treePointer parent, int insert);
void insertRight(treePointer parent, int insert);
treePointer insucc(treePointer tree);
void writeTree(treePointer tree);

treePointer createNode(int data)
{
    treePointer node = (treePointer)malloc(sizeof(struct threadedTree));

    node->data = data;
    node->leftchild = NULL;
    node->rightchild = NULL;
    node->leftThread = true;
    node->rightThread = true;

    return node;
}

void createTree(int data, char *direction, int insert, treePointer *tree)
{
    treePointer temp = *tree;

    if(temp == NULL)
    {
        return;
    }

    treePointer stack[100];
    int top = -1;

    stack[++top] = temp;
    while(top != -1)
    {
        temp = stack[top--];

        if(temp->data == data)
        {
            break;
        }

        if(!temp->rightThread)
        {
            stack[++top] = temp->rightchild;
        }
        if(!temp->leftThread)
        {
            stack[++top] = temp->leftchild;
        }
    }
    
    if(strcmp(direction,"left") == 0)
    {
        insertLeft(temp,insert);
    }
    
    if(strcmp(direction,"right") == 0)
    {
        insertRight(temp,insert);
    }
}

void insertLeft(treePointer parent, int insert)
{
    treePointer child = createNode(insert);

    child->leftchild = parent->leftchild;
    child->leftThread = true;
    child->rightchild = parent;
    child->rightThread = true;
    
    parent->leftchild = child;
    parent->leftThread = false;
}

void insertRight(treePointer parent, int insert)
{
    treePointer child = createNode(insert);

    child->rightchild = parent->rightchild;
    child->rightThread = true;
    child->leftchild = parent;
    child->leftThread = true;

    parent->rightchild = child;
    parent->rightThread = false;
}

treePointer insucc(treePointer tree)
{
    treePointer temp = tree;

    if(temp == NULL)
    {
        return NULL;
    }

    while(!temp->leftThread)
    {
        temp = temp->leftchild;
    }

    return temp;
}

void writeTree(treePointer tree)
{
    treePointer temp = insucc(tree);

    while(temp != NULL)
    {
        printf("%d ",temp->data);

        if(temp->rightThread)
        {
            temp = temp->rightchild;
        }
        else
        {
            temp = insucc(temp->rightchild);
        }
    }
}

int main()
{
    treePointer tree; 
    int n, rootData;

    scanf("%d %d",&n,&rootData);

    //initialize the tree
    tree = createNode(rootData);

    int node, insertData;
    char insertDir[10];

    for(int i = 0 ; i < n ; i++)
    {
        scanf("%d %s %d",&node,insertDir,&insertData);

        createTree(node,insertDir,insertData,&tree);
    }

    writeTree(tree);

    return 0;
}