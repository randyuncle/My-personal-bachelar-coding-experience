#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#define MAX 10000

typedef struct
{
    int key;
}
element;

element heap[MAX][MAX];
int heapNum = 0;
int n[MAX];

void priorityChg(int numOfHeap, int node, int newKey);
void swap(int numOfHeap, int smallest, int node);
void minHeapify(int numOfHeap, int node, int length);

void insertHeap(int numOfHeap, int inKey)
{
    if(n[numOfHeap] == (MAX - 1))
    {
        printf("this heap is full!");
        return;
    }

    heap[numOfHeap][++n[numOfHeap]].key = inKey;

    priorityChg(numOfHeap, n[numOfHeap], inKey);
}

void deleteMin(int numOfHeap)
{
    element first;

    if(n[numOfHeap] == 0)
    {
        printf("this heap is empty!");
        return;
    }

    first = heap[numOfHeap][1];
    heap[numOfHeap][1].key = heap[numOfHeap][n[numOfHeap]--].key;

    minHeapify(numOfHeap, 1, n[numOfHeap]);

    printf("%d has been deleted!\n",first.key);
}

void priorityChg(int numOfHeap, int node, int newKey)
{
    int presKey = heap[numOfHeap][node].key;

    if(newKey > presKey)
    {
        return;
    }

    heap[numOfHeap][node].key = newKey;

    while(node > 1 && heap[numOfHeap][(int)node/2].key > heap[numOfHeap][node].key)
    {
        swap(numOfHeap, node, (int)node/2);
        node = node / 2;
    }
}

void minHeapify(int numOfHeap, int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if(left <= length && heap[numOfHeap][left].key < heap[numOfHeap][node].key)
    {
        smallest = left;
    } 
    else
    {
        smallest = node;
    }

    if(right <= length && heap[numOfHeap][right].key < heap[numOfHeap][smallest].key)
    {
        smallest = right;
    }

    if(smallest != node)
    {
        swap(numOfHeap, smallest, node);
        minHeapify(numOfHeap, smallest, length);
    }
}

void swap(int numOfHeap, int smallest, int node)
{
    int temp = heap[numOfHeap][smallest].key;
    heap[numOfHeap][smallest].key = heap[numOfHeap][node].key;
    heap[numOfHeap][node].key = temp;
}

void printHeap(int numOfHeap)
{
    for(int i = 1 ; i <= n[numOfHeap] ; i++)
    {
        printf("%d ",heap[numOfHeap][i].key);
    }
}

int main()
{
    printf("Welcome to the min_heap generator!\n");
    printf("The commands (noted with numbers) :\n");
    printf("1、create min heap\n");
    printf("2、remove the key with the lowest value (which is the root node)\n");
    printf("3、change the priority of an arbitrary element\n");
    printf("4、insert an element into the heap\n");
    printf("5、quit\n");
    printf("---------------------------------------------------------------------\n");
    
    while(true)
    {
        int motion, heapName, numOfHeap, name[MAX];

        printf("enter your motion after the '$' (Enter the number 1~5 ONLY):\n");
        printf("$ ");
        scanf("%d",&motion);

        if(motion == 1)
        {
            /**
             * build heap 
             */
            printf("enter the heap name and the number of nodes:\n$ ");
            scanf("%d %d",&name[heapNum],&n[heapNum]);
            numOfHeap = heapNum;

            for(int i = 1 ; i <= n[heapNum] ; i++)
            {
                scanf("%d",&heap[heapNum][i].key);
            }

            for(int i = (int)n[heapNum]/2 ; i >= 1 ; i--)
            {
                minHeapify(numOfHeap, i, n[heapNum]);
            }

            heapNum++;
        }
        else if(motion == 2)
        {
            printf("enter the heap name to delete:\n$ ");
            scanf("%d",&heapName);

            for(int i = 0 ; i < heapNum ; i++)
            {
                if(name[i] == heapName)
                {
                    numOfHeap = i;
                    break;
                }
            }

            deleteMin(numOfHeap);
        }
        else if(motion == 3)
        {
            int node, newKey;
            printf("enter three numbers represent 'heap name', 'the changing node', and 'new priority key', respectively:\n$ ");
            scanf("%d %d %d",&heapName,&node,&newKey);

            for(int i = 0 ; i < heapNum ; i++)
            {
                if(name[i] == heapName)
                {
                    numOfHeap = i;
                    break;
                }
            }

            priorityChg(numOfHeap,node,newKey);
        }
        else if(motion == 4)
        {
            int insertKey;
            printf("enter two numbers represent 'heap name' and 'new key', respectively:\n$ ");
            scanf("%d %d",&heapName,&insertKey);

            for(int i = 0 ; i < heapNum ; i++)
            {
                if(name[i] == heapName)
                {
                    numOfHeap = i;
                    break;
                }
            }

            insertHeap(numOfHeap, insertKey);
            printf("insertion success\n");
        }
        else if(motion == 5)
        {
            printf("System end");
            break;
        }

        printf("the #'%d' heap you has done in this command is now being: ",name[numOfHeap]);
        printHeap(numOfHeap);
        printf("\n");
    }

    return 0;
}