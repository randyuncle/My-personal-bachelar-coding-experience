#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void merge(vector<int> &sort, int front, int mid, int end){
    vector<int> left(sort.begin() + front, sort.begin() + mid + 1);
    vector<int> right(sort.begin() + mid + 1, sort.begin() + end + 1);

    left.push_back(numeric_limits<int>::max());
    right.push_back(numeric_limits<int>::max());

    int indL = 0, indR = 0;
    for(int i = front ; i <= end ; i++){
        if(left[indL] <= right[indR]) sort[i] = left[indL++];
        else sort[i] = right[indR++];
    }

    return;
}
/**
 * @brief merge sort method (recursive)
 * 
 * @param sort 
 * @param n 
 */
void mergeSort(vector<int> &sort, int front,int end){
    if(front < end){
        int mid = (front + end) / 2;
        mergeSort(sort, front, mid);
        mergeSort(sort, mid + 1, end);
        merge(sort, front, mid, end);
    }

    return;
}

int findGap(vector<int> &sort, int n){
    int max = 0;
    for(int i = 0 ; i < (n - 1) ; i++){
        int gap = sort[i + 1] - sort[i];
        if(max < gap) max = gap;
    }
    return max;
}

int main(){
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    
    int n, max_gap;
    cin >> n;

    vector <int> sort(n + 10);
    for(int i = 0 ; i < n ; i++) cin >> sort[i];

    if(n < 2){
        cout << 0;
    }else{
        mergeSort(sort, 0, n - 1);
        cout << findGap(sort, n);
    }

    return 0;
}