#include <stdio.h>
#include <stdlib.h>
#define n 8000

// Ignore this
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*)){exit(1);}
// Ignore this

void quicksort(int a[], int low, int high);
int main(void) {
  int a[n];
  for (int i = 0; i < n; i++)
    scanf("%d", &a[i]);
  quicksort(a, 0, n - 1);
  for (int i = 0; i < n; i++)
    printf("%d ", a[i]);
  return 0;
}
void quicksort(int a[], int low,int high){
    int m;
    if (low >= high) 
        return;
    int element = a[low],l = low,h = high;
    while(1){
        while(l < h && element <= a[h]){
            h--;
        }
        if (l >= h) 
            break;
        a[l++] = a[h];
        while (l < h && a[l] <= element){
            l++;
        }
        if (l >= h)
            break;
        a[h--] = a[l];
    }
    a[h] = element;
    m = h;
    quicksort(a, low, m - 1);
    quicksort(a, m + 1, high);
}
