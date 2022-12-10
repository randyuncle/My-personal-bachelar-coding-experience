#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX 4001

int max;
int heapPer[MAX];
/**
 * @brief initialization of the sorts(without "Insertion Sort")
 * 
 * @param sort 
 * @param n 
 */
void initialize(int sort[], int n){
    for(int i = 1 ; i <= n ; i++){
        sort[i] = i;
    }
    return;
}

/************************worst cases generator****************************/

void swap(int sort[], int i, int j){
    int temp = sort[i];
    sort[i] = sort[j];
    sort[j] = temp;
    return;
}

/**
 * @brief random genertor for the  worst case of the heap sort and quick sort
 * 
 * @param sort 
 * @param n 
 */
void permute(int sort[], int n){
    for(int i = n ; i >= 2 ; i--){
        int j = rand() % i + 1;
        heapPer[i] = j;
        swap(sort, i, j);
    }
    return;
}

void worstInsertion(int sort[], int n){
    int up = n;
    for(int i = 1 ; i <= n ; i++)
        sort[i] = up--;
    return;
}

void join(int sort[], int left[], int right[], int l, int mid, int r){
    int i;
    for(i = 1 ; i <= mid - l + 1 ; i++)
        sort[i] = left[i];
    i--;
    for(int j = 1 ; j <= r - mid ; j++)
        sort[i + j] = right[j];
    return;
}
void split(int sort[], int left[], int right[], int l, int mid, int r){
    for(int i = 1 ; i <= mid - l + 1 ; i++)
        left[i] = sort[i*2 - 1];
    for(int i = 1 ; i <= r - mid ; i++)
        right[i] = sort[i*2];
    return;
}
void worstMerge(int sort[], int l, int r){
    if(l < r){
        int mid = l + (r - l) / 2;
        int left[mid - l + 2];
        int right[r - mid + 1];

        split(sort, left, right, l, mid, r);
        worstMerge(left, l, mid);
        worstMerge(right, mid + 1, r);

        join(sort, left, right, l, mid, r);
    }

    return;
}

/*********************sorting functions***************************/

void insert(int element, int sort[], int i){
    for(sort[0] = element ; (element < sort[i]) ; i--)
        sort[i + 1] = sort[i];
    sort[i + 1] = element;
    return;
}
/**
 * @brief insertion sort method
 * 
 * @param sort 
 * @param n 
 */
void insertionSort(int sort[], int n){
    for(int i = 2 ; i <= n ; i++){
        int temp = sort[i];
        insert(temp, sort, i - 1);
    }
    return;
}

void partition(int sort[], int l, int r, int *i, int *j){
    (*i) = l - 1;
    (*j) = r;
    int p = l - r, q = r;
    int v = sort[r];

    while(true){
        while(sort[++(*i)] < v);

        while(v < sort[--(*j)]){
            if((*j) == l)
                break;
        }

        if((*i) >= (*j))
            break;
        
        swap(sort, (*i), (*j));

        if(sort[(*i) == v]){
            p++;
            swap(sort, (*i), p);
        }

        if(sort[(*j)] == v){
            q--;
            swap(sort, (*j), p);
        }
    }

    swap(sort, (*i), r);

    (*j) = (*i) - 1;
    for(int k = l ; k < p ; k++, (*j)--)
        swap(sort, (*j), k);

    (*i) += 1;
    for(int k = r - 1 ; k > q ; k--, (*i)++)
        swap(sort, (*i), k);

    return;
}
/**
 * @brief quick sort method(median of three)
 * 
 * @param sort 
 * @param l 
 * @param r 
 */
void quickSort(int sort[], int l, int r){
    if(r <= l)
        return;
    int i, j;

    partition(sort, l, r, &i, &j);

    quickSort(sort, l, j);
    quickSort(sort, i, r);

    return;
}

void merge(int initList[], int mergeList[], int i, int m, int n){
    int j, k, t;
    j = m + 1;
    k = i;

    while((i <= m) && (j <= n)){
        if(initList[i] <= initList[j])
            mergeList[k++] = initList[i++];
        else
            mergeList[k++] = initList[j++];
    }

    if(i > m){
        for(t = j ; t <= n ; t++)
            mergeList[t] = initList[t];
    }else{
        for(t = i ; t <= m ; t++)
            mergeList[k + t - i] = initList[t];
    }

    return;
}
void mergePass(int initList[], int mergeList[], int n, int curr){
    int i, j;
    for(i = 1 ; i <= n - 2*curr + 1 ; i += 2*curr)
        merge(initList, mergeList, i, i + curr - 1,i + 2*curr -1);
    
    if((i + curr - 1) < n)
        merge(initList, mergeList, i, i + curr - 1, n);
    else{
        for(j = i ; j <= n ; j++)
            mergeList[j] = initList[j];
    }

    return;
}
/**
 * @brief merge sort method (iterative)
 * 
 * @param sort 
 * @param n 
 */
void mergeSort(int sort[], int n){
    int curr = 1;
    int extra[max];

    while(curr < n){
        mergePass(sort, extra, n, curr);
        curr *= 2;
        mergePass(extra, sort, n, curr);
        curr *= 2;
    }

    return;
}

void adjust(int sort[], int root, int n){
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
void heapSort(int sort[], int n){
    for(int i = n/2 ; i > 0 ; i--)
        adjust(sort, i, n);
    for(int i = n-1 ; i > 0 ; i--){
        swap(sort, 1, i + 1);
        adjust(sort, 1, i);
    }
    return;
}

/************************main*******************************/

/*
int main(){
    int cases[5] = {500, 1000, 2000, 3000, 4000};
    clock_t begin, end;
    srand((unsigned)time(NULL));

    for(int round = 0 ; round < 5 ; round++){
        max = cases[round];
        int n = cases[round];
        int insertion[MAX], quick[MAX], merge[MAX], heap[MAX];

        worstInsertion(insertion, n);//insertion sort

        initialize(quick, n);//quick sort
        permute(quick, n);

        initialize(merge, n); //merge sort
        worstMerge(merge, 1, n);

        initialize(heap, n); //heap sort
        permute(heap, n);
        
        //following are the sorting area
        begin = clock();
        insertionSort(insertion, n);
        end = clock();
        printf("%f\n", (double)(begin - end));

        begin = clock();
        quickSort(quick, 1, n);
        end = clock();
        printf("%f\n", (double)(begin - end));

        begin = clock();
        mergeSort(merge, n);
        end = clock();
        printf("%f\n", (double)(begin - end));

        begin = clock();
        heapSort(heap, n);
        end = clock();
        printf("%f\n", (double)(begin - end));
    }

    return 0;
}
*/
int main(){
    int cases[5] = {500, 1000, 2000, 3000, 4000};
    clock_t begin, end;
    srand((unsigned)time(NULL));

    for(int round = 0 ; round < 5 ; round++){
        max = cases[round];
        int n = cases[round];
        int heapInit[MAX], quickInit[MAX];
        double ins = 0.0000, qui = 0.0000, mer = 0.0000, h = 0.0000;

        initialize(quickInit, n);//quick sort init. random
        permute(quickInit, n);

        initialize(heapInit, n); //heap sort init. random
        permute(heapInit, n);

        /*
        printf("#%d random permutation for heap : ", max);
        for(int i = 0 ; i < max ; i++)
            printf("%d ",heapPer[i]);
        printf("\n");
        */
        for(int i = 0 ; i < 1000 ; i++){
            int insertion[MAX], quick[MAX], merge[MAX], heap[MAX];

            worstInsertion(insertion, n);//insertion sort

            for(int j = 1 ; j <= n ; j++)//quick sort
                quick[j] = quickInit[j];

            initialize(merge, n); //merge sort
            worstMerge(merge, 1, n);
            /*
            if(i == 0){ //print worst merge sort
                for(int j = 0 ; j < max ; j++)
                    printf("%d ",merge[j]);
                printf("\n");
            }
            */

            for(int j = 1 ; j <= n ; j++) //heap sort
                heap[j] = heapInit[j];
            
            //following are the sorting area
            begin = clock();
            insertionSort(insertion, n);
            end = clock();
            ins += (double)(end - begin);

            begin = clock();
            quickSort(quick, 1, n);
            end = clock();
            qui += (double)(end - begin);

            begin = clock();
            mergeSort(merge, n);
            end = clock();
            mer += (double)(end - begin);

            begin = clock();
            heapSort(heap, n);
            end = clock();
            h += (double)(end - begin);
        }

        printf("%f (ms)\n", ins / 1000);
        printf("%f (ms)\n", qui / 1000);
        printf("%f (ms)\n", mer / 1000);
        printf("%f (ms)\n", h / 1000);
    }
}