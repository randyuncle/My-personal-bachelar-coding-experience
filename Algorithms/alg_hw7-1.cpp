#include <iostream>

using namespace std;

//dynamic partition method O(n*sum)
int findMin(int n, int array[], int sum){
    int dp[n + 1][sum + 1]; //BOOLEAN SAVE! The former is the number of elements, while the latter is the sum of the elements
    //initialize n = 0 and sum = 0, respectively
    for(int i = 0 ; i <= n ; i++) dp[i][0] = 1; //sum = 0, choose
    for(int i = 1 ; i <= sum ; i++) dp[0][i] = 0; //n = 0, doesn't choose

    for(int i = 1 ; i <= n ; i++){
        for(int j = 1 ; j <= sum ; j++){
            //determine to include or exclude the element
            dp[i][j] = dp[i - 1][j]; //exclude
            if(array[i - 1] <= j) //include
                if(!dp[i][j]) dp[i][j] = dp[i - 1][j - array[i-1]];
        }
    }

    int diff = INT32_MAX;
    for(int i = sum / 2 ; i >= 0 ; i--){
        if(dp[n][i]){
            diff = sum - 2 * i; break;
        }
    }

    return diff;
}

int main(){
    int n;
    cin >> n;
    
    int array[n], sum = 0;
    for(int i = 0 ; i < n ; i++) cin >> array[i], sum += array[i];

    cout << findMin(n, array, sum);

    return 0;
}