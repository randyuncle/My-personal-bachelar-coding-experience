#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 500000

typedef struct
{
    long int data;
}element;

element heap[MAX + 1];
int last = 1;

long int print[MAX/2 + 1][MAX + 1];
int printLast[MAX + 1], count = 0; 

void insertHeap(long int x)
{
    int currentNode, gp, lcgp, rcgp;
    bool done;

    currentNode = ++last;
    if((last % 2 == 1) && x < heap[last - 1].data)
    {
        heap[last].data = heap[last - 1].data;
        currentNode--;
    }

    done = false;

    while(!done && currentNode >= 4)
    {
        gp = currentNode / 4; //grandparent
        lcgp = 2 * gp; //left child of grandparent
        rcgp = lcgp + 1; //right child of grandparent

        if(x < heap[lcgp].data)
        {
            heap[currentNode].data = heap[lcgp].data;
            currentNode = lcgp;
        }
        else if(x > heap[rcgp].data)
        {
            heap[currentNode].data = heap[rcgp].data;
            currentNode = rcgp;
        }
        else
        {
            done = true;
        }
    }

    heap[currentNode].data = x;

    return;
}

void swap(long int smallest, long int node)
{
    int temp = heap[smallest].data;
    heap[smallest].data = heap[node].data;
    heap[node].data = temp;
}

void minHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 2, smallest;

    if(left <= length && heap[left].data < heap[node].data)
    {
        smallest = left;
    } 
    else
    {
        smallest = node;
    }

    if(right <= length && heap[right].data < heap[smallest].data)
    {
        smallest = right;
    }

    if(smallest != node)
    {
        swap(smallest, node);

        if(heap[smallest].data > heap[smallest + 1].data)
        {
            swap(smallest + 1, smallest);
        }
        
        minHeapify(smallest, length);
    }
}

void maxHeapify(int node, int length)
{
    int left = 2 * node - 1, right = 2 * node + 1 , biggest;

    if(left <= length && heap[left].data > heap[node].data)
    {
        biggest = left;
    } 
    else
    {
        biggest = node;
    }

    if(right <= length && heap[right].data > heap[biggest].data)
    {
        biggest = right;
    }

    if(biggest != node)
    {
        swap(node, biggest);

        if(heap[biggest].data < heap[biggest - 1].data)
        {
            swap(biggest, biggest - 1);
        }
        
        maxHeapify(biggest, length);
    }
}

void deleteMin()
{
    heap[2].data = heap[last--].data;

    minHeapify(2, last);
}

void deleteMax()
{
    heap[3].data = heap[last--].data;

    maxHeapify(3, last);
}

void printHeap()
{  
    for(int i = 1 ; i <= last ; i++)
    {
        print[count][i] = heap[i].data;
    }

    printLast[count++] = last;
    return;
}

int main()
{
    char command[10], *p;
    long int x;

    while(true)
    {
        scanf("%s",command);
        p = command;

        if(strcmp(p,"insert") == 0)
        {
            scanf("%ld",&x);
            insertHeap(x);
        }
        else if(strcmp(p,"delete") == 0)
        {
            scanf("%s",command);
            p = command;

            if(strcmp(p,"min") == 0)
            {
                deleteMin();
            }
            else
            {
                deleteMax();
            }
        }
        else if(strcmp(p,"show") == 0)
        {
            printHeap();
        }
        else if(strcmp(p,"quit") == 0)
        {
            break;
        }
    }

    for(int i = 0 ; i < count ; i++)
    {
        int nextLine = 1, nodes = 0;;

        for(int j = 1 ; j <= printLast[i] ; j++)
        {
            if(j == 1)
            {
                printf("NULL");
            }
            else
            {
                printf("%ld ",print[i][j]);
            }
                
            if((j == (nextLine + nodes)) && (j != printLast[i]))
            {
                nodes += nextLine;
                nextLine = nextLine * 2;
                printf("\n");
            }
        }

        printf("\n");
    }

    return 0;
}
