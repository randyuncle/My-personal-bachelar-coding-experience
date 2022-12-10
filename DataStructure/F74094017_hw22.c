#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100000

/**
 * @brief fibonacci heap construction
 * 
 */
typedef struct fibonacciHeap* heap;
struct fibonacciHeap
{
    long key;
    long value;
    int degree;
    heap parent;
    heap leftSibling;
    heap rightSibling;
    heap child;
    bool marked;
    bool visited;
};
heap h;

long int print[MAX][2];
int printCount = 0;

//the number nodes 
int nodes = 0;

heap createHeap(long int key, long int val){
    heap Heap = (heap)malloc(sizeof(struct fibonacciHeap));
    Heap->key = key;
    Heap->value = val;
    Heap->degree = 0;
    Heap->leftSibling = Heap;
    Heap->rightSibling = Heap;
    Heap->child = NULL;
    Heap->parent = NULL;
    Heap->marked = false;
    Heap->visited = false;

    return Heap;
}

heap heapUnion(heap a, heap b){
    heap new;
    new = a;

    heap temp1, temp2;
    temp1 = h->rightSibling;
    temp2 = b->leftSibling;

    new->rightSibling->leftSibling = b->leftSibling;
    new->rightSibling = b;
    b->leftSibling = h;
    temp2->rightSibling = temp1;
    if(!a || (b && a->key > b->key))
        new = b;
    return new;
}

void heapLink(heap a, heap b){
    a->rightSibling->leftSibling = a->leftSibling;
    a->leftSibling->rightSibling = a->rightSibling;
    if(b->rightSibling == b)
        h = b;    
    a->leftSibling = a;
    a->rightSibling = a;
    a->parent = b;
    if(!b->child)
        b->child = a;
    a->rightSibling = b->child;
    a->leftSibling = b->child->leftSibling;
    b->child->leftSibling->rightSibling = a;
    b->child->leftSibling = a;
    if(a->key < b->child->key)
        b->child = a;
    b->degree++;
    return;
}

void insertHeap(long int x, long int val){
    heap new = createHeap(x, val);   
    if(!h)
        h = new;
    else
    {
        h->leftSibling->rightSibling = new;
        new->rightSibling = h;
        new->leftSibling = h->leftSibling;
        h->leftSibling = new;
        if(new->key < h->key)
            h = new;
    }
    nodes++;
    return;
}

int NodesDegree(long int num){
    int count = 0;
    do
    {
        num /= 2;
        count++;
    }while(num >= 1);
    
    return count;
}

void consolidate(){
    int degree, maxDegree = NodesDegree(nodes) + 1;
    heap node[maxDegree], a, b, c;

    for(int i = 0 ; i < maxDegree ; i++)
        node[i] = NULL;
    a = h;
    c = a;
    do{
        degree = a->degree;
        c = c->rightSibling;
        while((node[degree] != NULL) && (degree < maxDegree)){
            b = node[degree];
            if(a->key > b->key){
                heap change = a;
                a = b;
                b = change;
            }

            if(b == h)
                h = a;
                
            if(a != b)
                heapLink(b, a);

            if(b->rightSibling == a)
                h = a;
            node[degree] = NULL;
            degree++;
        }
        node[degree] = a;
        a = a->rightSibling;
    }while(a != h);

    h = NULL;

    for(int i = 0 ; i < maxDegree ; i++){
        if(node[i]){
            node[i]->leftSibling = node[i];
            node[i]->rightSibling = node[i];
            if(!h)
                h = node[i];
            else{
                h->leftSibling->rightSibling = node[i];
                node[i]->rightSibling = h;
                node[i]->leftSibling = h->leftSibling;
                h->leftSibling = node[i];

                if(node[i]->key < h->key)
                    h = node[i];
            }

            if(!h)
                h = node[i];
            else if(node[i]->key < h->key)
                h = node[i];
        }
    }
}

/**
 * @brief delete the min in the f-heap and return the deleted min node
 * 
 * @return heap 
 */
heap extractMin(){
    if(!h)
        return NULL;

    heap temp = h;
    heap parent = temp;
    heap x;

    if(temp->child){
        x = temp->child;
        do{
            parent = x->rightSibling;
            h->leftSibling->rightSibling = x;
            x->rightSibling = h;
            x->leftSibling = h->leftSibling;
            h->leftSibling = x;

            if(x->key < h->key)
                h = x;

            x->parent = NULL;
            x = parent;
        }while(parent != temp->child);
    }

    temp->leftSibling->rightSibling = temp->rightSibling;
    temp->rightSibling->leftSibling = temp->leftSibling;
    h = temp->rightSibling;

    if((temp == temp->rightSibling) && !temp->child)
        h = NULL;
    else{
        h = temp->rightSibling;
        consolidate();
    }

    nodes--;
    return temp;
}

/**
 * @brief the cutting method
 * 
 * @param decrease 
 * @param parent 
 */
void cut(heap decrease, heap parent){
    if(decrease == decrease->rightSibling)
        parent->child = NULL;

    decrease->leftSibling->rightSibling = decrease->rightSibling;
    decrease->rightSibling->leftSibling = decrease->leftSibling;

    if(decrease == parent->child)
        parent->child = decrease->rightSibling;

    parent->degree--;
    
    decrease->leftSibling = decrease;
    decrease->rightSibling = decrease;
    h->leftSibling->rightSibling = decrease;
    decrease->rightSibling = h;
    decrease->leftSibling = h->leftSibling;
    h->leftSibling = decrease;

    decrease->parent = NULL;
    decrease->marked = true;
}

/**
 * @brief find if the mark is true or not
 * 
 * @param parent 
 */
void cascadingCut(heap parent){
    heap temp = parent->parent;
    if(temp){
        if(!parent->marked)
            parent->marked = true;
        else{
            cut(parent, temp);
            cascadingCut(temp);
        }
    }
}

void decreaseKey(heap decrease, long int newKey){
    if(!h || !decrease)
        return;

    decrease->key = newKey;
    heap parent = decrease->parent;
    if(parent && (decrease->key < parent->key)){
        cut(decrease, parent);
        cascadingCut(parent);
    }

    if(decrease->key < h->key)
        h = decrease;
}

void findNode(heap n, long int key, long int val, long int newKey){
    heap find = n;
    heap temp = NULL;
    heap found = NULL;
    find->visited = true;
    if((find->key == key) && (find->value == val)){
        find->visited = false;
        temp = find;
        decreaseKey(temp, key - newKey);
    }

    if(!temp)
    {
        if(find->child)
            findNode(find->child, key, val, newKey);

        if(!find->rightSibling->visited)
            findNode(find->rightSibling, key, val, newKey);
    }

    find->visited = false;
    temp = found;
    return;
}

void delete(int key, int val){
    findNode(h, key, val, 100000);
    heap del = extractMin();
    free(del);
    return;
}

int main()
{
    char command[10], *p;
    long int x, val, y;

    while(true)
    {
        scanf("%s",command);
        p = command;

        if(strcmp(p,"insert") == 0)
        {
            scanf("%ld %ld",&x,&val);
            insertHeap(x, val);
        }
        else if(strcmp(p,"delete") == 0)
        {
            scanf("%ld %ld",&x,&val);
            delete(x, val);
        }
        else if(strcmp(p,"extract") == 0)
        {
            heap min = extractMin();
            if(min != NULL)
            {
                print[printCount][0] = min->key;
                print[printCount++][1] = min->value;
            }
        }
        else if(strcmp(p,"decrease") == 0)
        {
            scanf("%ld %ld %ld",&x,&val,&y);
            findNode(h, x, val, y);
        }
        else if(strcmp(p,"quit") == 0)
        {
            for(int i = 0 ; i < printCount ; i++)
            {
                printf("(%ld)%ld\n", print[i][0], print[i][1]);
            }
            break;
        }
    }

    return 0;
}