#include <iostream>
#include <cstring>
#define MAX 50001

using namespace std;

int findMax(int n, int array[][2], int sum){
    int dp[MAX]; 
    //initialize
    memset(dp, 0, sizeof(dp));

    for(int i = 0 ; i < n ; i++){
        for(int j = sum ; j >= array[i][1] ; j--){
            //determine to include or exclude the element
            dp[j] = max(dp[j], dp[j - array[i][1]] + array[i][0]);
        }
    }

    return dp[sum];
}

int main(){
    int n, sum;
    cin >> n >> sum;
    
    int array[n][2]; //0->score ; 1->time
    for(int i = 0 ; i < n ; i++) cin >> array[i][0] >> array[i][1];

    cout << findMax(n, array, sum);

    return 0;
}