#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define MAX 10000000

struct cases
{
    int motion;
    int num; // only the insert will have the num that >= 0
};

/**
 * struct of the leftiest tree
 */ 
typedef struct leftist* leftistTree;
struct leftist
{
    leftistTree leftChild;
    int key;
    leftistTree rightChild;
    int shortest;
};
leftistTree leftisttree;

/**
 * @brief struct of the min heap
 * 
 */
typedef struct
{
    int key;
}
element;
element heap[7][MAX];
int heapNum = 0;
int heapLen = 0;

void minMeld(leftistTree *a, leftistTree *b);
void minUnion(leftistTree *a, leftistTree *b);
void swap(leftistTree *a, leftistTree *b);
void priorityChg(int node, int newKey);
void minHeapify(int node, int length);
void swapHeap(int smallest, int node);

void insertTreeNode(int data)
{
    leftistTree node = (leftistTree)malloc(sizeof(struct leftist));

    node->key = data;
    node->shortest = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;

    minMeld(&leftisttree, &node); 
}

void deleteMinTreeNode()
{
    if(leftisttree)
    {
        minMeld(&leftisttree->leftChild, &leftisttree->rightChild);

        leftisttree = leftisttree->leftChild;
    }
    else 
    {
        //printf("this tree is empty!\n");
    }
}

/**
 * @brief melding the leftist tree
 * 
 * @param a 
 * @param b 
 */
void minMeld(leftistTree *a, leftistTree *b)
{
    if((!*a))
    {
        *a = *b;
    }
    else if(*b)
    {
        minUnion(a, b);
    }

    *b = NULL;
}

/**
 * @brief leftist tree merge and swap condition hander
 * 
 * @param a 
 * @param b 
 */
void minUnion(leftistTree *a, leftistTree *b)
{
    if((*a)->key > (*b)->key)
    {
        swap(&(*a), &(*b));
    }

    if(!(*a)->rightChild)
    {
        (*a)->rightChild = *b;
    }
    else
    {
        minUnion(&(*a)->rightChild, b);
    }

    if(!(*a)->leftChild)
    {
        (*a)->leftChild = (*a)->rightChild;
        (*a)->rightChild = NULL;
    }
    else if((*a)->leftChild->shortest < (*a)->rightChild->shortest)
    {
        swap(&(*a)->leftChild, &(*a)->rightChild);
    }

    if((*a)->rightChild)
    {
        (*a)->shortest = (*a)->rightChild->shortest + 1;
    }
}
/**
 * @brief swap for the leftist tree
 * 
 * @param a 
 * @param b 
 */
void swap(leftistTree *a, leftistTree *b)
{
    leftistTree temp = *b;
    (*b) = *a;
    (*a) = temp;
}

/*-----------------------------------------------------------------*/

/**
 * @brief heap insert
 * 
 * @param numOfHeap 
 * @param inKey 
 */
void insertHeap(int inKey)
{
    if(heapLen == (MAX - 1))
    {
        //printf("this heap is full!");
        return;
    }

    heap[heapNum][++heapLen].key = inKey;

    priorityChg(heapLen, inKey);
}

/**
 * @brief min heap delete
 * 
 * @param numOfHeap 
 */
void deleteMin()
{
    if(heapLen == 0)
    {
        //printf("this heap is empty!\n");
        return;
    }

    heap[heapNum][1].key = heap[heapNum][heapLen--].key;

    minHeapify(1, heapLen);

    //printf("%d has been deleted!\n",first.key);
}

/**
 * @brief the priority of the whole heap is change (ex. insert the node to the back of the heap)
 * 
 * @param node 
 * @param newKey 
 */
void priorityChg(int node, int newKey)
{
    int presKey = heap[heapNum][node].key;

    if(newKey > presKey)
    {
        return;
    }

    heap[heapNum][node].key = newKey;

    while(node > 1 && heap[heapNum][(int)node/2].key > heap[heapNum][node].key)
    {
        swapHeap(node, (int)node/2);
        node = node / 2;
    }
}

/**
 * @brief min heap sortion
 * 
 * @param node 
 * @param length 
 */
void minHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if(left <= length && heap[heapNum][left].key < heap[heapNum][node].key)
    {
        smallest = left;
    } 
    else
    {
        smallest = node;
    }

    if(right <= length && heap[heapNum][right].key < heap[heapNum][smallest].key)
    {
        smallest = right;
    }

    if(smallest != node)
    {
        swapHeap(smallest, node);
        minHeapify(smallest, length);
    }
}

/**
 * @brief swap for heap
 * 
 * @param smallest 
 * @param node 
 */
void swapHeap(int smallest, int node)
{
    int temp = heap[heapNum][smallest].key;
    heap[heapNum][smallest].key = heap[heapNum][node].key;
    heap[heapNum][node].key = temp;
}

int main()
{
    struct cases m[5000];
    clock_t begin, end;
    srand((unsigned)time(NULL));

    //insert and delete random list
    for(int i = 0 ; i < 5000 ; i++)
    {
        int motion = rand() % 2;

        if(motion == 0)
        {
            //insert
            m[i].motion = 0;
            m[i].num = rand() % 100000;
        }
        else
        {
            //delete min
            m[i].motion = 1;
            m[i].num = -1;
        }
    }

    /**
     * @brief n = 100 : first test
     * 
     */
    leftisttree = NULL;

    //initialize of the n = 100
    for(int i = 0 ; i < 100 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    for(int i = (int)heapLen/2 ; i >= 1 ; i--)
    {
        minHeapify(i, heapLen);
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#1 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#1 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 500 : second test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 500
    for(int i = 0 ; i < 500 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#2 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#2 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 1000 : third test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 1000
    for(int i = 0 ; i < 1000 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#3 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#3 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 2000 : forth test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 2000
    for(int i = 0 ; i < 2000 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#4 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#4 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 3000 : fifth test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 3000
    for(int i = 0 ; i < 3000 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#5 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#5 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 4000 : sixth test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 4000
    for(int i = 0 ; i < 4000 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#6 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#6 min heap costs : %f \n",(double)(end - begin)/5000);

    /**
     * @brief n = 5000 : seventh test
     * 
     */
    leftisttree = NULL;
    heapLen = 0;
    heapNum++;

    //initialize of the n = 5000
    for(int i = 0 ; i < 5000 ; i++)
    {
        int data = rand() % 100000;

        insertTreeNode(data);
        heap[heapNum][i + 1].key = data;
        heapLen++;
    }

    //leftist tree's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertHeap(m[i].num);
        }
        else
        {
            deleteMin();
        }
    }  
    end = clock();
    printf("#7 leftist tree costs : %f \n",(double)(end - begin)/5000);

    //min heap's sequence test
    begin = clock();
    for(int i = 0 ; i < 5000 ; i++)
    {
        if(m[i].motion == 0)
        {
            insertTreeNode(m[i].num);
        }
        else
        {
            deleteMinTreeNode();
        }
    }  
    end = clock();
    printf("#7 min heap costs : %f \n",(double)(end - begin)/5000);
}
