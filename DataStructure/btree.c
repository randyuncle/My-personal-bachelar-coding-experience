#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 2
#define MIN 0

/**
 * @brief struct of b tree
 * ## At here, key serves as a value. 
 * 
 */
typedef struct node *btreePointer;
struct node
{
    int datas[MAX + 2]; //the usable place will be 1 & 2 (3 for split)
    int count;
    btreePointer parent;
    btreePointer child[MAX + 2];
    bool leaf;
};
btreePointer btree;

//remove handler
bool isRemoved = false;
int traveredValue = -1;

char printWord[MAX][10];
int printNum[MAX][2], count = 0; //for remove and get -> printNum[count] = -1 => not found

btreePointer createNode(int data, btreePointer parent)
{
    btreePointer node = (btreePointer)malloc(sizeof(struct node));

    node->count = 1;
    node->datas[1] = data;
    node->datas[2] = -1;
    node->datas[3] = -1;
    node->parent = parent;
    node->child[0] = NULL;
    node->child[1] = NULL;
    node->child[2] = NULL;  
    node->child[3] = NULL;
    node->leaf = true;

    return node;
}

void split(btreePointer node)
{
    btreePointer parent = node->parent;

    if(!parent)
    {
        btree = createNode(node->datas[2], NULL);
        btree->child[0] = node;
        btree->child[1] = createNode(node->datas[3], btree);
        node->parent = btree;
        btree->leaf = false;
        node->count -= 2;
        return;
    }

    parent->count++;
    if(node == parent->child[0]) //at child[0]
    {
        for(int i = parent->count ; i > 1 ; i--)
        {
            parent->datas[i] = parent->datas[i - 1];
            parent->child[i] = parent->child[i - 1];
        }

        parent->datas[1] = node->datas[2];
        parent->child[0] = node;
        parent->child[1] = createNode(node->datas[3], parent);
        parent->child[1]->child[0] = node->child[2];
        parent->child[1]->child[1] = node->child[3];
        node->count -= 2;
    }
    else if(node == parent->child[1]) //at child[1]
    {
        for(int i = parent->count ; i > 2 ; i--)
        {
            parent->datas[i] = parent->datas[i - 1];
            parent->child[i] = parent->child[i - 1];
        }

        parent->datas[2] = node->datas[2];
        parent->child[1] = node;
        parent->child[2] = createNode(node->datas[3], parent);
        parent->child[2]->child[0] = node->child[2];
        parent->child[2]->child[1] = node->child[3];
        node->count -= 2;
    }
    else //at child[2]
    {
        parent->datas[parent->count] = node->datas[2];
        parent->child[2] = node;
        parent->child[3] = createNode(node->datas[3], parent);
        parent->child[3]->child[0] = node->child[2];
        parent->child[3]->child[1] = node->child[3];
        node->count -= 2;
    }

    node->datas[3] = -1;
    node->datas[2] = -1;

    if(parent->count > MAX)
    {
        split(parent);
    }

    return;
}

void addValue(btreePointer curr, int data)
{
    int position; //the position the data should be in this node whether it is max
    if(curr->leaf)
    {
        if(curr->count < MAX)
        {
            if(curr->datas[curr->count] > data)
            {
                curr->datas[2] = curr->datas[1];
                curr->datas[1] = data;
                curr->child[2] = NULL;
                return;
            }

            curr->datas[++(curr->count)] = data;
            curr->child[2] = NULL;
            return;
        }

        //split the node if it max
        curr->count++;
        for(position = curr->count ; (position > 1) && (data < curr->datas[position - 1]) ; position--);
        for(int i = curr->count ; i > position ; i--)
        {
            curr->datas[i] = curr->datas[i - 1];
            curr->child[i] = curr->child[i - 1];
        }
        curr->datas[position] = data;
        split(curr);
        return;
    }

    //recursion insert
    for(position = curr->count ; (position > MIN) && (data < curr->datas[position]) ; position--);
    addValue(curr->child[position], data);
    return;
}

void insert(int data)
{
    if(!btree)
    {
        btree = createNode(data, NULL);
        return;
    }

    addValue(btree, data);
    return;
}

btreePointer mergeChild(btreePointer child1, btreePointer child2)
{
    child1->datas[++(child1->count)] = child2->datas[1];

    for(int i = (child1->count - 1) ; i <= child1->count ; i++)
    {
        if(child1->child[i])
        {
            child1->child[i]->datas[2] = child2->child[i - 1]->datas[1];
        }
        else
        {
            child1->child[i] = child2->child[i - 1];
        }
    }

    if(child2->child[2])
    {
        child1->datas[3] = child2->child[2]->datas[1];
        child1->child[3] = child2->child[2];
        child1->count++;
        split(child1);
    }
    
    return child1;
}

/**
 * @brief adjust the tree after the deletion
 * 
 * @param node 
 */
void adjust(btreePointer node)
{
    if(node->count < 1) //empty node
    {
        if(node->child[1])
        {
            if(node->child[1]->count > 1)
            {
                node->datas[1] = node->child[1]->datas[1];
                node->child[1]->datas[1] = node->child[1]->datas[0];
                node->count--;
                adjust(node->child[1]);
            }
            else
            {
                if(node->child[0]->count > 1)
                {
                    node->datas[1] = node->child[0]->datas[2];
                    node->count--;
                    adjust(node->child[0]);
                }
                else
                {
                    node = mergeChild(node->child[0], node->child[1]);
                }
            }
        }
        else
        {
            adjust(node->parent);
        }
    }
    else //not an empty node (last a data)
    {
        if(node->datas[1]) //data is on the lefthand side
        {
            if(node->child[2]) //child[2] has node
            {
                if(node->child[2]->count == 1)//child[2] only has one
                {
                    if(node->child[1]->count > 1) //child[1] is able to support the tree
                    {
                        node->datas[++(node->count)] = node->child[1]->datas[(node->child[1]->count)--];
                        adjust(node->child[1]);
                    }
                    else //child[1] isn't able to support the tree => do the merge
                    {
                        node->child[1] = mergeChild(node->child[1], node->child[2]);
                    }
                }
            }
            else //child[2] has no nodes
            {
                if(!node->child[1] && node->child[0])
                {
                    if(node->parent->count == 1)
                    {
                        if(node->parent->child[1]->count > 1)
                        {
                            node->child[1] = node->parent;
                            node->child[2] = node->parent->child[2];
                            node->parent = node->parent->parent;
                            node->datas[2] = node->child[2]->datas[1];
                            node->child[2]->datas[2] = node->child[2]->datas[1];
                            if(node->child[2]->child[0])
                            {
                                node->child[1] = mergeChild(node->child[1], node->child[2]->child[0]);
                            }
                            for(int i = 0 ; (i < node->child[2]->count) && node->child[2]->child[i + 1] ; i++)
                            {
                                node->child[2]->child[i] = node->child[2]->child[i + 1];
                            }
                        }
                        else
                        {
                            node->child[1] = node->parent->child[1];
                            node->child[2] = node->parent->child[2];
                        }
                    }
                    else
                    {
                        int position;
                        for(position = 0 ; node != node->parent->child[position] ; position++);
                    }
                }
            }
        }
    }
}

void remove(btreePointer node, int position)
{
    btreePointer parent = node->parent;

    if(node->leaf)
    {
        node->datas[position] = -1;
        node->count--;
        if(node->count < 1)
        {
            node = NULL;
        }

        adjust(parent);
    }
    else
    {
        node->datas[position] = -1;
        node->count--;
        adjust(node);
    }
}

btreePointer search(int data, int *position, btreePointer node)
{
    if(!node)
    {
        return NULL;
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
            return node;
        }
    }

    search(data, position, node->child[*position]);
    return NULL;
}

void delete(btreePointer tree, int data)
{
    int position;
    btreePointer node = search(data, &position, tree);

    if(!node)
    {
        return;
    }

    remove(node, position);

    isRemoved = true;
    return;
}

int main()
{
    btreePointer find;
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
            find = search(key, &positon, btree);

            strcpy(printWord[count], "get");
            if(find)
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

    /*
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
    */

    return 0;
}