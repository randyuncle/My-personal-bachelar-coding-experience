#include <iostream>
#include <vector>
#define MAX 1000001

using namespace std;

vector<int> array(MAX);
vector<int> heap(MAX);

//heap sort
void swap(vector<int> &sort, int i, int j){
    int temp = sort[i];
    sort[i] = sort[j];
    sort[j] = temp;
    return;
}
void adjust(vector<int> &sort, int root, int n){
    int child, temp;
    temp = sort[root];
    child = 2 * root;
    while(child <= n){
        if((child < n) && (sort[child] < sort[child + 1])) child++;
        if(temp > sort[child]) break;
        else{
            sort[child / 2] = sort[child];
            child *= 2;
        }
    }
    sort[child / 2] = temp;
    return;
}
/**
 * @brief heap sort method (iterative)
 * 
 * @param sort 
 * @param n 
 */
void heapSort(vector<int> &sort, int n){
    for(int i = n/2 ; i > 0 ; i--) adjust(sort, i, n);
    for(int i = n-1 ; i > 0 ; i--){
        swap(sort, 1, i + 1);
        adjust(sort, 1, i);
    }
    return;
}

//max heap
void Swap(int smallest, int node) {
    int temp = heap[smallest];
    heap[smallest] = heap[node];
    heap[node] = temp;
    return;
}
void maxHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if (left <= length && heap[left] > heap[node]) smallest = left;
    else smallest = node;

    if (right <= length && heap[right] > heap[smallest]) smallest = right;

    if (smallest != node) {
        Swap(smallest, node);
        maxHeapify(smallest, length);
    }
    return;
}

int main(){
    int n;
    cin >> n;

    for(int i = 1 ; i <= n ; i++) cin >> heap[i], array[i] = heap[i];

    for(int i = (int)n / 2; i >= 1; i--) maxHeapify(i, n);
    for(int i = 1 ; i <= n ; i++) cout << heap[i] << " "; cout << endl;

    heapSort(array, n);
    for(int i = 1 ; i <= n ; i++) cout << array[i] << " ";

    return 0;
}