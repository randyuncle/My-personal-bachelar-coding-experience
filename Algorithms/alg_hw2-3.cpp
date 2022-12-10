#include <iostream>
#include <string>
#include <algorithm>
#define MAX 300001

using namespace std;

struct element{
    int x, y, idx;
};
element array[MAX];
int ranks[MAX];

bool cmp(element const& left, element const& right){
    if(left.x != right.x) return left.x < right.x;
    return left.y < right.y;
}

void merge(int front, int mid, int end){
    int i = front, j = mid + 1, k = front, count = 0;
    while(i <= mid && j <= end){
        if(array[i].y <= array[j].y){
            i++;
            count++;
        }else ranks[array[j++].idx] += count;
    }

    while(j <= end) ranks[array[j++].idx] += count;

    return;
}

void DAC(int front, int end){
    if(front < end){
        int mid = (front + end) / 2;
        DAC(front, mid);
        DAC(mid + 1, end);
        merge(front, mid, end);
    }
    return;
}

int main(){
    int n;
    cin >> n;
    for(int i = 0 ; i < n ; i++){
        cin >> array[i].x >> array[i].y;
        array[i].idx = i;
        ranks[i] = 0;
    }

    sort(array, array + n, &cmp);
    DAC(0, n - 1);

    for(int i = 0 ; i < n ; i++) cout << ranks[i] << " ";

    return 0;
}