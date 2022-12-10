#include <stdio.h>
#include <stdlib.h> 
#define MAX 1025

int heap[MAX];

void swap(long int smallest, long int node){
    int temp = heap[smallest];
    heap[smallest] = heap[node];
    heap[node] = temp;
    return;
}

void minHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if(left <= length && heap[left] < heap[node]) smallest = left;
    else smallest = node;

    if(right <= length && heap[right] < heap[smallest]) smallest = right;

    if(smallest != node){
        swap(smallest, node);
        minHeapify(smallest, length);
    }
    return;
}

int main(){
    int n;
    scanf("%d", &n);

    for(int i = 1 ; i <= n ; i++) scanf("%d", &heap[i]);;
    for(int i = n / 2 ; i >= 1 ; i--) minHeapify(i, n);
    for(int i = 1 ; i <= n ; i++) printf("%d ", heap[i]);
    return 0;
}