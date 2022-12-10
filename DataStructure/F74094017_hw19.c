#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#define MAX 1000000
//#define INT_MAX 9999999

typedef struct node *nodepointer;

typedef struct node
{
    int adjnode;
    int weight;
    nodepointer next;
};

nodepointer adjlists[MAX];
int distances[MAX], visited[MAX];

/**
 * @brief create the linked list of every adjacency node
 * 
 * @param insnode 
 * @param data 
 * @param weight 
 */
void createnode(int insnode, int data, int weight)
{
    nodepointer newnode = (nodepointer)malloc(sizeof(struct node));
    nodepointer temp;

    newnode->adjnode = data;
    newnode->weight = weight;
    newnode->next = NULL;

    if(!adjlists[insnode])
    {
        adjlists[insnode] = newnode;
    }
    else
    {
        temp = adjlists[insnode];
        while(temp->next)
        {
            temp = temp->next;
        }
        temp->next = newnode;
    }
}

/**
 * @brief short path by "single source single path dfs strategy"
 * @brief using dfs to apply the O(n) short path reading  
 * 
 * @param root 
 */
void shortpath(int root)
{
    visited[root]++;

    while(adjlists[root])
    {
        if(visited[adjlists[root]->adjnode] == 0)
        {
            distances[adjlists[root]->adjnode] = distances[root] + adjlists[root]->weight;
            shortpath(adjlists[root]->adjnode);
        }

        adjlists[root] = adjlists[root]->next;
    }
}

int main()
{
    int V, from, to, weight, root;
    scanf("%d",&V);

    //initialization
    for(int i = 0 ; i < V ; i++)
    {
        visited[i] = 0;
        distances[i] = 0;
    }

    //adjacency list construction
    for(int i = 0 ; i < V - 1 ; i++)
    {
        scanf("%d %d %d",&from,&to,&weight);

        createnode(from - 1, to - 1, weight);
        
        createnode(to - 1, from - 1, weight);
    }

    scanf("%d",&root);

    shortpath(root - 1);
    
    //print
    for(int i = 0 ; i < V ; i++)
    {
        printf("%d %d\n",i + 1,distances[i]);
    }

    return 0;
}