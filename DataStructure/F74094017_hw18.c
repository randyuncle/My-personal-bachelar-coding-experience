#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#define MAX 2000000

//Edge strategy
typedef struct edge
{
    int from,to,w;
}
edge;
 
//stream the input edges 
typedef struct edgelist
{
    edge data[MAX];
    int n;
}
edgelist;
 
edgelist elist;

//subset for the union-find
typedef struct subset 
{
    int parent;
    int rank;
}
subset;

//parent list for the union-find
subset subsets[MAX];

//the total weight the question wants 
//(use long int in order saving the large number from the 10^6 input datas)
long int totalWeight = 0;

//total vertices in need
int v;

void minHeapify(int node, int length);
void swap(int smallest, int node);
int find(int i);

/**
 * union-find by ranks
 */
void Union(int i, int j, int w)
{
    int aRoot = find(i);
    int bRoot = find(j);

    if(aRoot != bRoot)
    {
        //compare the ranks
        if(subsets[aRoot].rank > subsets[bRoot].rank)
        {
            subsets[bRoot].parent = aRoot;
        }
        else if(subsets[aRoot].rank < subsets[bRoot].rank)
        {
            subsets[aRoot].parent = bRoot;
        }
        else
        {
            subsets[bRoot].parent = aRoot;
            subsets[aRoot].rank++;
        }

        v--;
        totalWeight += w;
    }
}

/**
 * find strategy for the union-find
 */
int find(int i)
{
    if(subsets[i].parent != i)
    {
        subsets[i].parent = find(subsets[i].parent);
    }

    return subsets[i].parent;
}

/**
 * pop from the min heap (and do the minheapify again)
 */ 
edge deleteMin()
{
    edge first;

    if(elist.n == 0)
    {
        first.from = -1;
        first.to = -1;
        first.from = -1;
        //false
        return first;
    }

    first = elist.data[1];
    elist.data[1].from = elist.data[elist.n].from;
    elist.data[1].to = elist.data[elist.n].to;
    elist.data[1].w = elist.data[elist.n--].w;

    minHeapify(1, elist.n);

    return first;
}

/**
 * min heap sort change strategy
 */
void minHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if(left <= length && elist.data[left].w < elist.data[node].w)
    {
        smallest = left;
    } 
    else
    {
        smallest = node;
    }

    if(right <= length && elist.data[right].w < elist.data[smallest].w)
    {
        smallest = right;
    }

    if(smallest != node)
    {
        swap(smallest, node);
        minHeapify(smallest, length);
    }
}

/**
 * swap of the nodes' properties
 */ 
void swap(int smallest, int node)
{
    int tempfrom = elist.data[smallest].from;
    int tempTo = elist.data[smallest].to;
    int tempW = elist.data[smallest].w;

    elist.data[smallest].from = elist.data[node].from;
    elist.data[smallest].to = elist.data[node].to;
    elist.data[smallest].w = elist.data[node].w;

    elist.data[node].from = tempfrom;
    elist.data[node].to = tempTo;
    elist.data[node].w = tempW;
}

int main()
{
    int e;
    scanf("%d %d",&v,&e);
    elist.n = e;

    for (int i = 0; i < e; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    for(int i = 1 ; i <= e ; i++)
    {
        scanf("%d %d %d",&elist.data[i].from,&elist.data[i].to,&elist.data[i].w);
    }

    /**
     * start the kruskal's algorithm
     */ 

    //min heap sort
    for(int i = (int)e/2 ; i >= 1 ; i--)
    {
        minHeapify(i, e);
    }

    //union-find strategy (by the tree ranks)
    for(int i = 1 ; i <= e && v > 0 ; i++)
    {
        edge edges = deleteMin();
        if(edges.w != 0)
        {
            Union(edges.from, edges.to, edges.w);
        }
        else
        {
            break;
        }
    }

    //print the total weights
    printf("%ld",totalWeight);

    return 0;
}