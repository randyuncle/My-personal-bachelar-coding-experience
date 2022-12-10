#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int merge(vector<int> &sort, int front, int mid, int end){
    int inverse = 0;

    vector<int> left(sort.begin() + front, sort.begin() + mid + 1);
    vector<int> right(sort.begin() + mid + 1, sort.begin() + end + 1);

    left.push_back(numeric_limits<int>::max());
    right.push_back(numeric_limits<int>::max());

    int indL = 0, indR = 0;
    for(int i = front ; i <= end ; i++){
        if(left[indL] <= right[indR]){
            sort[i] = left[indL++];
        }else{
            sort[i] = right[indR++];
            if(indL <= (mid - front)) inverse = (inverse + (mid - front + 1) - indL) % 524287;
        }
    }

    return inverse;
}
/**
 * @brief merge sort method (recursive)
 * 
 * @param sort 
 * @param n 
 */
int mergeSort(vector<int> &sort, int front,int end){
    int inverse = 0;
    if(front < end){
        int mid = (front + end) / 2;
        inverse = (inverse + mergeSort(sort, front, mid)) % 524287;
        inverse = (inverse + mergeSort(sort, mid + 1, end)) % 524287;
        inverse = (inverse + merge(sort, front, mid, end)) % 524287;
    }

    return inverse;
}

int main(){
    int n, inverse;
    cin >> n;

    vector <int> num(n + 10); 

    for(int i = 0 ; i < n ; i++){
        cin >> num[i];
    }

    inverse = mergeSort(num, 0, n-1);

    cout << inverse << endl;
    return 0;
}