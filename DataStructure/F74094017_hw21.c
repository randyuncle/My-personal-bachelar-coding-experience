#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX 100000

struct cases{
    int motion;
    int num;
};

/**
 * @brief struct of the leftiest tree
 * 
 */
typedef struct leftist* leftistTree;
typedef struct leftist
{
    leftistTree leftChild;
    int key;
    leftistTree rightChild;
    int shortest;
};
leftistTree leftisttree;

/**
 * @brief struct of the binomial heap
 * 
 */
typedef struct binomialHeap* heap;
typedef struct binomialHeap{
    int data;
    int degree;
    heap parent;
    heap child;
    heap sibling;
};
heap h;

void minMeld(leftistTree *a, leftistTree *b);
void minUnion(leftistTree *a, leftistTree *b);
void swap(leftistTree *a, leftistTree *b);

heap newNode(int key){
    heap node = (heap)malloc(sizeof(struct binomialHeap));
    
    node->data = key;
    node->degree = 0;
    node->child = NULL;
    node->parent = NULL;
    node->sibling = NULL;

    return node;
}

void heaplink(heap a, heap b)
{
    a->parent = b;
    a->sibling = b->child;
    b->child = a;
    b->degree++;
}

heap heapMerge(heap h1, heap h2)
{
    heap root = NULL;

    if(h1)
    {
        if(h2 && (h1->degree <= h2->degree))
        {
            root = h1;
            h1 = h1->sibling;
        }
        else if(h2 && (h1->degree > h2->degree))
        {
            root = h2;
            h2 = h2->sibling;
        }
        else //if h2 doesn't exist
        {
            return h1;
        }
    }
    else //if h1 doesn't exist
    {
        return h2;
    }

    heap temp = root;

    while(h1 && h2)
    {
        if(h1->degree <= h2->degree)
        {
            temp->sibling = h1;
            h1 = h1->sibling;
        }
        else
        {
            temp->sibling = h2;
            h2 = h2->sibling;
        }

        temp = temp->sibling;
    }

    if(h1)
    {
        temp->sibling = h1;
    }
    else
    {
        temp->sibling = h2;
    }

    return root;
}

heap heapUnion(heap h1, heap h2)
{
    heap pre = NULL, next, curr;
    heap temp = heapMerge(h1, h2);

    if(h == NULL)
    {
        return temp;
    }

    curr = temp;
    next = curr->sibling;

    while(next)
    {
        if((curr->degree != next->degree) || (next->sibling && (next->sibling->degree == curr->degree)))
        {
            pre = curr;
            curr = next;
        }
        else
        {
            if(curr->data < next->data)
            {
                curr->sibling = next->sibling;
                heaplink(next, curr);
            }
            else
            {
                if(!pre)
                {
                    temp = next;
                }
                else
                {
                    pre->sibling = next;
                }

                heaplink(curr, next);
                curr = next;
            }
        }

        next = curr->sibling;

    }

    return temp;
}

void insertHeap(int inKey)
{
    heap newHeap = newNode(inKey);

    h = heapUnion(h, newHeap);
}

void heapReconstruct(heap min, heap prev)
{
    if(h == min)
    {
        h = min->sibling;
    }
    else if(min && prev)
    {
        prev->sibling = min->sibling;
    }

    heap temp = NULL;//the new head in the b-heap
    heap curr = min->child;
    heap next = NULL;

    while(curr)
    {
        next = curr->sibling;
        curr->sibling = temp;
        curr->parent = NULL;
        temp = curr;
        curr = next;
    }

    h = heapUnion(h, temp);

    return;//make sure the b-heap, h, get the result from the heapUnion (kinda weird = =) 
}

void deleteMin(heap h1)
{
    if(!h1)
    {
        return;
    }

    heap min = h1;
    heap min_prev = NULL;
    heap next = h1->sibling;
    heap next_prev = min;

    while(next)
    {
        if(next->data < min->data)
        {
            min = next;
            min_prev = next_prev;
        }

        next_prev = next;
        next = next->sibling;
    }

    heapReconstruct(min, min_prev);

    return;
}

/*-----------------------------------------------------------------------*/

void insertTreeNode(int data){
    leftistTree node = (leftistTree)malloc(sizeof(struct leftist));

    node->key = data;
    node->shortest = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;

    minMeld(&leftisttree, &node);
}

void deleteRoot(){
    if(leftisttree){
        minMeld(&leftisttree->leftChild, &leftisttree->rightChild);

        leftisttree = leftisttree->leftChild;
    }
}

void minMeld(leftistTree *a, leftistTree *b){
    if(!(*a)){
        *a = *b;
    }else if(*b){
        minUnion(a, b);
    }

    *b = NULL;
}

void minUnion(leftistTree *a, leftistTree *b){
    if((*a)->key > (*b)->key){
        swap(&(*a), &(*b));
    }

    if(!(*a)->rightChild){
        (*a)->rightChild = *b;
    }else{
        minUnion(&(*a)->rightChild, b);
    }

    if(!(*a)->leftChild){
        (*a)->leftChild = (*a)->rightChild;
        (*a)->rightChild = NULL;
    }else if((*a)->leftChild->shortest < (*a)->rightChild->shortest){
        swap(&(*a)->leftChild, &(*a)->rightChild);
    }

    if((*a)->rightChild){
        (*a)->shortest = (*a)->rightChild->shortest + 1;
    }
}

void swap(leftistTree *a, leftistTree *b)
{
    leftistTree temp = *b;
    (*b) = *a;
    (*a) = temp;
}

int main(){
    int testNum[7] = {100, 500, 1000, 2000, 3000, 4000, 5000};
    struct cases m[5000];
    clock_t begin, end;
    srand((unsigned)time(NULL));

    for(int i = 0 ; i < 5000 ; i++)
    {
        int motion = rand() % 2;

        if(motion)
        {
            m[i].motion = 1;
            m[i].num = -1;
        }else
        {
            m[i].motion = 0;
            m[i].num = rand() % 100000;
        }
    }

    //test function
    for(int i = 0 ; i < 7 ; i++)
    {
        leftisttree = NULL;
        h = NULL;

        //initialization
        for(int j = 0 ; j < testNum[i] ; j++)
        {
            int data = rand() % 100000;

            insertTreeNode(data);
            insertHeap(data);
        }

        //binary heap test
        begin = clock();
        for(int j = 0 ; j < 5000 ; j++)
        {
            if(!m[j].motion)
            {
                insertHeap(m[j].num);
            }
            else
            {
                deleteMin(h);
            }
        }
        end = clock();
        printf("#%d binomial heap costs : %f \n",i,(double)(end - begin)/5000);

        //leftist tree test
        begin = clock();
        for(int j = 0 ; j < 5000 ; j++)
        {
            if(!m[j].motion)
            {
                insertTreeNode(m[j].num);
            }
            else
            {
                deleteRoot();
            }
        }  
        end = clock();
        printf("#%d leftist tree costs : %f \n",i,(double)(end - begin)/5000);
    }

    return 0;
}
