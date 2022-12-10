#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int select(vector<int>&list, int l, int r, int n);

int partition(vector<int>&list, int l, int r, int p, int n){
    vector<int> small, big;
    for(int i = l ; i <= r ; i++) {
        if(p > list[i]) small.push_back(list[i]);
        if(p < list[i]) big.push_back(list[i]);
    }

    int newPivot = small.size();

    if(n > newPivot) 
        return select(big, 0, big.size() - 1, n - newPivot - 1);
    else if(n < newPivot)
        return select(small, 0, small.size() - 1, n);

    return p;
}

/**
 * @brief find the median of group 5
 * 
 * @param list 
 * @param l 
 * @param r 
 * @return int(value)
 */
int partition5(vector<int>&list, int l, int r){
    int arr[list.size()];
    for(int i = l ; i <= r ; i++) arr[i] = list[i];

    for(int i = l + 1 ; i <= r ; i++)
        for(int j = i ; j > l ; j--)
            if(arr[j - 1] > arr[j]) swap(arr[j - 1], arr[j]);
    
    int mid = floor((r + l) / 2);
    return arr[mid];
}

/**
 * @brief find the pivot of "median of median"
 * 
 * @param list 
 * @param l 
 * @param r 
 * @return int (value)
 */
int pivot(vector<int>&list, int l, int r){
    vector<int> median;
    for(int i = l ; i <= r ; i += 5){
        if(i > r) break;

        int subright = i + 4;
        if(subright > r) subright = r;

        int m = partition5(list, i, subright);
        median.push_back(m);
    }

    int mid = floor((median.size() - 1) / 2);
    return select(median, 0, median.size() - 1, mid);
}

int select(vector<int>&list, int l, int r, int n){
    if(l == r) return list[l];

    int p = pivot(list, l, r);
    cout << "pivot: " << p << endl;

    return partition(list, l, r, p, n);
}

int main(){
    int n;
    cin >> n;

    vector<int> v;
    for(int i = 0 ; i < n ; i++){
        int in; cin >> in;
        v.push_back(in);
    }

    int ans = select(v, 0, n - 1, floor((n - 1) / 2));
    cout << "answer: " << ans;

    return 0;
}