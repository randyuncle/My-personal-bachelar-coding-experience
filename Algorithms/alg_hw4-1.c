#include <stdio.h>
#include <stdlib.h> 
#define ll long long int

void swap(ll sort[], int i, int j){
    int temp = sort[i];
    sort[i] = sort[j];
    sort[j] = temp;
    return;
}

void adjust(ll sort[], int root, int n){
    int child, temp;
    temp = sort[root];
    child = 2 * root;
    while(child <= n){
        if((child < n) && (sort[child] < sort[child + 1]))
            child++;
        if(temp > sort[child])
            break;
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
void heapSort(ll sort[], int n){
    for(int i = n/2 ; i > 0 ; i--)
        adjust(sort, i, n);
    for(int i = n-1 ; i > 0 ; i--){
        swap(sort, 1, i + 1);
        adjust(sort, 1, i);
    }
    return;
}

int findGap(ll sort[], int n){
    ll max = 0;
    for(int i = 1 ; i < n ; i++){
        ll gap = sort[i + 1] - sort[i];
        if(max < gap) max = gap;
    }
    return max;
}

int main(){    
    int n;
    scanf("%d", &n);

    ll sort[n + 1];
    for(int i = 1 ; i <= n ; i++) scanf("%lld", &sort[i]);

    if(n < 2){
        printf("0");
    }else{
        heapSort(sort, n);
        printf("%d", findGap(sort, n));
    }

    return 0;
}