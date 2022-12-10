#include <iostream>
#define MAX 50001

using namespace std;

int array[MAX];

void swap(int sort[], int i, int j){
    int temp = sort[i];
    sort[i] = sort[j];
    sort[j] = temp;
    return;
}
void adjust(int sort[], int root, int n){
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
void heapSort(int sort[], int n){
    for(int i = n/2 ; i > 0 ; i--) adjust(sort, i, n);
    for(int i = n-1 ; i > 0 ; i--){
        swap(sort, 1, i + 1);
        adjust(sort, 1, i);
    }
    return;
}

int main(){
    int n;
    cin >> n;

    for(int i = 1 ; i <= n ; i++) cin >> array[i];

    heapSort(array, n);

    for(int i = 1 ; i <= n ; i++) cout << array[i] << " ";
    return 0;
}