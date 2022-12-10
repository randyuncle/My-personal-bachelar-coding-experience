#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 1000

/**
 * @brief struct of b tree (with BUGS)
 * ## At here, key serves as a value. 
 * 
 */
typedef struct node *btreePointer;
struct node
{
    int datas[4]; //the usable place will be 1 & 2
    int count;
    btreePointer child[4];
};
btreePointer btree;

//search
bool isInTree = false;

//remove handler
bool isRemoved = false;
int traveredValue = -1;

char printWord[MAX][10];
int printNum[MAX][2], count = 0; //for remove and get -> printNum[count] = -1 => not found

btreePointer createNode(int data, btreePointer child)
{
    btreePointer node = (btreePointer)malloc(sizeof(struct node));

    node->count = 1;
    node->datas[1] = data;
    node->child[0] = btree;
    node->child[1] = child;
    node->child[2] = NULL;
    node->child[3] = NULL;  
    node->child[4] = NULL;

    return node;
}

void insertNode(int data, int position, btreePointer node, btreePointer child)
{
    int i = node->count;
    
    while(i > position)
    {
        node->datas[i + 1] = node->datas[i];
        node->child[i + 1] = node->child[i];
        i--;
    }

    node->datas[i + 1] = data;
    node->child[i + 1] = child;
    node->count++;

    return ;
}

void splitNode(int data, int *upData, int position, btreePointer node, btreePointer child, btreePointer *newNode)
{
    int mid;

    if(position > 2)
    {
        mid = 2 + 1;
    }
    else
    {
        mid = 2;
    }

    *newNode = (btreePointer)malloc(sizeof(struct node));
    (*newNode)->child[0] = NULL;
    (*newNode)->child[1] = NULL;
    (*newNode)->child[2] = NULL;
    (*newNode)->child[3] = NULL; 
    int i = mid + 1;

    while(i <= 3)
    {
        (*newNode)->datas[i - mid] = node->datas[i];
        (*newNode)->child[i - mid] = node->child[i];
        i++;
    }

    node->count = mid;
    (*newNode)->count = 3 - mid;

    if(position <= 2)
    {
        insertNode(data, position, *newNode, child);
    }
    else
    {
        insertNode(data, position - mid, *newNode, child);
    }

    *upData = node->datas[node->count];
    (*newNode)->child[0] = node->child[node->count];
    node->count--;

    return;
}

bool setValue(int data, int *upData, btreePointer node,btreePointer *child)
{
    int position;

    if(node == NULL)
    {
        *upData = data;
        *child = NULL;
        return true;
    }

    if(data < node->datas[1])
    {
        position = 0;
    }
    else
    {
        for(position = node->count ; (data < node->datas[position]) && position > 1 ; position--);
    }

    if(setValue(data, upData, node->child[position], child))
    {
        if(node->count < 3)
        {
            insertNode(*upData, position, node, *child);
        }
        else
        {
            splitNode(*upData, upData, position, node, *child, child);
            return true;
        }
    }

    return false;
}

void insert(int key)
{
    int upKey;
    btreePointer child;

    bool check = setValue(key, &upKey, btree, &child);

    if(check)
    {
        btree = createNode(key, child);
    }
}

/**
 * @brief copy successor
 * 
 * @param node 
 * @param position 
 */
void copy(btreePointer node, int position)
{
    btreePointer thecopy = node->child[position];

    for(; thecopy->child[0] != NULL ;)
    {
        thecopy = thecopy->child[0];
    }

    node->datas[position] = thecopy->datas[1];
    return;
}

void removeVal(btreePointer node, int position)
{
    int i = position + 1;

    while(i <= node->count)
    {
        node->datas[i - 1] = node->datas[i];
        node->child[i - 1] = node->child[i];
        i++;
    }

    node->datas[node->count] = -1;
    node->child[node->count] = NULL;
    node->count--;

    return ;
}

/**
 * @brief do the borrow right command for the bottom up deletion 
 * 
 * @param node 
 * @param position 
 */
void rightShift(btreePointer node, int position)
{
    btreePointer temp = node->child[position];
    int i = temp->count;

    while(i > 0)
    {
        temp->datas[i + 1] = temp->datas[i];
        temp->child[i + 1] = temp->child[i];
        i--;
    }

    temp->datas[1] = node->datas[position];
    temp->child[1] = temp->child[0];
    temp->count++;

    temp = node->child[position - 1];
    node->datas[position] = temp->datas[temp->count];
    node->child[position] = temp->child[temp->count];
    node->count--;

    return;
}

/**
 * @brief do the borrow left command for the bottom up deletion 
 * 
 * @param node 
 * @param position 
 */
void leftShift(btreePointer node, int position)
{
    btreePointer temp = node->child[position - 1];

    temp->count++;
    temp->datas[temp->count] = node->datas[position];
    temp->child[temp->count] = node->child[position]->child[0];

    temp = node->child[position];
    node->datas[position] = temp->datas[1];
    temp->child[0] = temp->child[1];
    temp->count--;

    for(int i = 0 ; i <= temp->count ; i++)
    {
        temp->datas[i] = temp->datas[i + 1];
        temp->child[i] = temp->child[i + 1];
    }

    return;
}

/**
 * @brief node merges (when the parent node is NULL after the deletion)
 * 
 * @param node 
 * @param position 
 */
void merge(btreePointer node, int position)
{
    btreePointer node1 = node->child[position], node2 = node->child[position - 1];

    node2->count++;
    node2->datas[node2->count] = node1->datas[0];
    node2->child[node2->count] = node1->child[0];

    for(int i = 1 ; (i <= node1->count) && (node2->count < MAX) ; i++)
    {
        node2->count++;
        node2->datas[node2->count] = node1->datas[i];
        node2->child[node2->count] = node1->child[i];
    }

    for(int i = position ; i < node->count ; i++)
    {
        node->datas[i] = node->datas[i + 1];
        node->child[i] = node->child[i + 1];
    }

    node->count--;
    node1 = NULL;

    return;
}

/**
 * @brief adjust the nodes after the deletion
 * 
 * @param node 
 * @param position 
 */
void adjust(btreePointer node, int position)
{
    if(!position)
    {
        if(node->child[1])
        {
            if((node->child[1]->count > 2))
            {
                leftShift(node, 1);
            }
            else
            {
                merge(node, 1);
            }
        }   
    }
    else
    {
        if(node->count != position)
        {
            if(node->child[position - 1]->count > 2)
            {
                rightShift(node, position);
            }
            else
            {
                if(node->child[position + 1]->count > 2)
                {
                    leftShift(node, position + 1);
                }
                else
                {
                    merge(node, position);
                }
            }
        }
        else
        {
            if(node->child[position - 1]->count > 2)
            {
                rightShift(node, position);
            }
            else
            {
                merge(node, position);
            }
        }
    }

    return;
}

/**
 * @brief to mention if the given data is in the nodes or not
 * 
 * @param data 
 * @param node 
 * @return true 
 * @return false 
 */
bool deleteValue(int data, btreePointer node)
{
    int position;
    bool check = false;

    if(node)
    {
        if(data < node->datas[1])
        {
            position = 0;
            check = false;
        }
        else
        {
            for(position = node->count ; (data < node->datas[position]) && (position > 1) ; position--);
            if(data == node->datas[position])
            {
                check = true;
            }
            else
            {
                check = false;
            }
        }
    }

    if(check)
    {
        if(node->child[position - 1])
        {
            copy(node, position);
            check = deleteValue(node->datas[position],node->child[position]);
            /*
            if(!check) printf("the given data isn't present in b tree");
            */
        }
        else
        {
            removeVal(node, position);
        }
    }
    else
    {
        if(node->child[position])
        {
            check = deleteValue(data, node->child[position]);
        }
        else
        {
            check = false;
        }
    }

    if(node->child[position])
    {
        if(node->child[position]->count < 2)
        {
            adjust(node, position);
        }
    }

    return check;
}

/**
 * @brief delete method
 * 
 * @param data 
 * @param node 
 */
void delete(int data, btreePointer node)
{
    btreePointer temp;

    if(!deleteValue(data, node))
    {
        //printf("not present");
        return;
    }
    else
    {
        if(node->count == 0)
        {
            temp = node;
            node = node->child[0];
            free(temp);
        }
    }

    btree = node;
    isRemoved = true;
    return;
}

void search(int data, int *position, btreePointer node)
{
    if(!node)
    {
        return;
    }

    if(data < node->datas[1])
    {
        *position = 0;
    }
    else
    {
        for(*position = node->count ; (data < node->datas[*position]) && (*position > 1) ; (*position)--);
        if(data == node->datas[*position])
        {
            isInTree = true;
            return;
        }
    }

    search(data, position, node->child[*position]);
    return;
}

/**
 * @brief inorder tranversal strategy
 * 
 */
void tranversal(int place, int *tranversed, btreePointer node)
{
    int i;
    if(node != NULL)
    {
        for(i = 0 ; (i < node->count) && !isInTree ; i++)
        {
            tranversal(place, tranversed, node->child[i]);
            
            if(!isInTree)
            {
                traveredValue = node->datas[i + 1];
                (*tranversed)++;

                if(*tranversed == place)
                {
                    isInTree = true;
                    return;
                }
            }
        }

        if(!isInTree)
        {
            tranversal(place, tranversed, node->child[i]);
        }
    }

    return;
}

int main()
{
    char command[10], *p;
    int num;
    scanf("%d",&num);
    
    for(int i = 0 ; i < num ; i++)
    {
        scanf("%s",command);
        p = command;

        if(strcmp(p,"add") == 0)
        {
            int key;
            scanf("%d", &key);
            insert(key);
            
            strcpy(printWord[count], "add");
            printNum[count++][0] = key;
        }
        else if(strcmp(p,"get") == 0)
        {   int key, positon;
            scanf("%d", &key);
            search(key, &positon, btree);

            strcpy(printWord[count], "get");
            if(isInTree)
            {
                printNum[count][0] = key;
                printNum[count++][1] = key;
            }
            else
            {
                printNum[count][0] = key;
                printNum[count++][1] = -1;
            }
            isInTree = false;
        }
        else if(strcmp(p,"getk") == 0)
        {
            int tranversed = 0, place;
            scanf("%d", &place);
            tranversal(place, &tranversed, btree);

            strcpy(printWord[count], "getk");
            if(isInTree)
            {
                printNum[count][0] = place;
                printNum[count++][1] = traveredValue;
            }
            else
            {
                printNum[count][0] = place;
                printNum[count++][1] = -1;
            }
            isInTree = false;
        }
        else if(strcmp(p,"remove") == 0)
        {
            int key;
            scanf("%d", &key);
            delete(key, btree);

            strcpy(printWord[count], "remove");
            if(isRemoved)
            {
                printNum[count][0] = key;
                printNum[count++][1] = key;
            }
            else
            {
                printNum[count][0] = key;
                printNum[count++][1] = -1;
            }
            isRemoved = false;
        }
        else if(strcmp(p,"removek") == 0)
        {
            int tranversed = 0, place;
            scanf("%d", &place);
            tranversal(place, &tranversed, btree);
            if(isInTree)
            {
                delete(traveredValue, btree);
            }

            strcpy(printWord[count], "removek");
            if(isRemoved)
            {
                printNum[count][0] = place;
                printNum[count++][1] = traveredValue;
            }
            else
            {
                printNum[count][0] = place;
                printNum[count++][1] = -1;
            }
            isRemoved = false;
            isInTree = false;
        }
    }

    for(int i = 0 ; i < count ; i++)
    {
        if(strcmp(printWord[i], "add") == 0)
        {
            printf("add(%d) = ok\n",printNum[i][0]);
        }
        else
        {
            if(printNum[i][1] == -1)
            {
                printf("%s(%d) = not found\n",printWord[i], printNum[i][0]);
            }
            else
            {
                printf("%s(%d) = %d\n", printWord[i], printNum[i][0], printNum[i][1]);
            }
        }
    }

    return 0;
}