#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int findMin(vector<int>&arr , int n){
    vector<int> dpInc(100010), dpDec(100010), dp(100010);
    int ans = 0;
    dpInc[0] = 1;
    dpDec[n - 1] = 1;

    for(int i = 1 ; i < n ; i++){
        dpInc[i] = 1;
        for(int j = 0 ; j < i ; j++){
            if(arr[i] > arr[j] && dpInc[i] < (dpInc[j] + 1))
                dpInc[i] = dpInc[j] + 1;
        }
    }
    for(int i = n - 2 ; i >= 0 ; i--){
        dpDec[i] = 1;
        for(int j = n - 1 ; j > i ; j--){
            if(arr[i] > arr[j] && dpDec[i] < (dpDec[j] + 1))
                dpDec[i] = dpDec[j] + 1;
        }
    }

    for(int i = 0 ; i < n ; i++){
        dp[i] = dpInc[i] + dpDec[i] - 2;
        ans = max(dp[i], ans);
    }
    return n - ans - 1;
}

int main(){
    int n;
    cin >> n;

    vector<int> v(100010);
    for(int i = 0 ; i < n ; i++) cin >> v[i];

    cout << findMin(v, n);
    return 0;
}