#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>

typedef struct node *treePointer;
typedef struct node
{
    char data;
    treePointer leftchild;
    treePointer rightchild;
};

int start = 1;

char str[100][200];
int strCou = 0,charCou = 0;

treePointer createNode(char n, treePointer *tree)
{
    treePointer temp = (treePointer)malloc(sizeof(struct node));

    temp->data = n;
    temp->leftchild = NULL;
    temp->rightchild = NULL;

    return temp;
}

treePointer createTree(char input[], treePointer *tree, int end)
{
    treePointer temp = *tree;

    if(start == end)
    {
        return NULL;
    }

    temp = createNode(input[start++],&temp);

    if(input[start] == '(')
    {
        start++;

        if(temp->data > input[start])
        {
            int change = temp->data;
            temp->data = input[start];
            input[start] = change;
        }

        temp->leftchild = createTree(input,&temp,end);
    }

    if(input[start] == ',')
    {
        start++;
        
        if(temp->data > input[start])
        {
            int change = temp->data;
            temp->data = input[start];
            input[start] = change;
        }

        temp->rightchild = createTree(input,&temp,end);
    }

    if(input[start] == ')')
    {
        start++;
    }

    return temp;
}

treePointer copyTree(treePointer origin)
{
    treePointer temp = (treePointer)malloc(sizeof(struct node));

    if(origin != NULL)
    {
        temp->leftchild = copyTree(origin->leftchild);
        temp->rightchild = copyTree(origin->rightchild);
        temp->data = origin->data;

        return temp;
    }

    return NULL;
}

bool equalChk(treePointer first, treePointer second)
{
    return ((!first && !second) || (first && second && (first->data == second->data) && equalChk(first->leftchild,second->leftchild) && equalChk(first->rightchild,second->rightchild)));
}

void cleanTree(treePointer node)
{
       if (node->leftchild)
       {
            cleanTree(node->leftchild);            
       }
       if (node->rightchild)
       {
            cleanTree(node->rightchild);
       }

       free(node);
}

void writeTree(treePointer tree)
{
    if (tree == NULL)
    {
        return;
    }
    
    str[strCou][charCou++] = tree->data;

    if(tree->leftchild != NULL)
    {
        str[strCou][charCou++] = '(';
        writeTree(tree->leftchild);
    }
    
    if(tree->rightchild != NULL)
    {
        str[strCou][charCou++] = ',';
        writeTree(tree->rightchild);
    }
    else
    {
        str[strCou][charCou++] = ')';
    }

}

int main()
{
    treePointer tree[50];
    int n,treeIdCou = 0,treeId[100],printCou = 0;
    char inputs[100];
    char *command;
    char* print[50];

    scanf("%d\n",&n);

    for(int i = 0 ; i < n ; i++)
    {
        char p[10];

        scanf("%s",p);

        command = p;

        if(strcmp(command,"read") == 0)
        {
            scanf("%d",&treeId[treeIdCou]);
            scanf("%s",inputs);

            int end = strlen(inputs) - 1;

            tree[treeIdCou] = createTree(inputs,&tree[treeIdCou],end);

            treeIdCou++;
            start = 1;

            print[printCou++] = "readed";
        }
        else if(strcmp(command,"copy") == 0)
        {
            int srcTreeId ;

            scanf("%d",&srcTreeId);
            scanf("%d",&treeId[treeIdCou]);

            int space;

            for(int j = 0 ; j < treeIdCou ; j++)
            {
                if(treeId[j] == srcTreeId)
                {
                    space = j;

                    break;
                }
            }

            tree[treeIdCou++] = copyTree(tree[space]);

            print[printCou++] = "copied";
        }
        else if(strcmp(command,"isequal") == 0)
        {
            int treeId1,treeId2;

            scanf("%d",&treeId1);
            scanf("%d",&treeId2);

            int space1;
            int space2;

            for(int j = 0 ; j < (treeIdCou-1) ; j++)
            {
                if(treeId[j] == treeId1)
                {
                    space1 = j;
                }
                else if(treeId[j] == treeId2)
                {
                    space2 = j;
                }
            }

            bool equal = equalChk(tree[space1],tree[space2]);

            if(equal)
            {
                print[printCou++] = "true";
            }
            else
            {
               print[printCou++] = "false";
            }
        }
        else if(strcmp(command,"clear") == 0)
        {
            int clTreeId;

            scanf("%d",&clTreeId);

            int space;

            for(int j = 0 ; j < (treeIdCou) ; j++)
            {
                if(treeId[j] == clTreeId)
                {
                    space = j;

                    break;
                }
            }

            cleanTree(tree[space]);

            for(int j = space ; j < (treeIdCou - 1) ; j++)
            {
                tree[j] = tree[j+1];
                treeId[j] = treeId[j+1];
            }

            treeIdCou--;

            print[printCou++] = "cleared";
        }
        else if(strcmp(command,"write") == 0)
        {
            int clTreeId;

            scanf("%d",&clTreeId);

            int space;

            for(int j = 0 ; j < (treeIdCou) ; j++)
            {
                if(treeId[j] == clTreeId)
                {
                    space = j;

                    break;
                }
            }

            str[strCou][charCou++] = '(';
            writeTree(tree[space]);

            charCou = 0;
            print[printCou++] = str[strCou++];
        }

    }

    for(int i = 0 ; i < printCou ; i++)
    {
        printf("%s",print[i]);
        printf("\n");
    }

    return 0;
}