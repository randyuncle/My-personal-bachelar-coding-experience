#include <iostream>
#include <vector>

using namespace std;

vector <int> array(2000001);
vector <int> rhs(2000001);

int main(){
    int n;
    cin >> n;

    for(int i = 0 ; i < n ; i++) cin >> array[i];

    int temp = INT32_MIN, maxCurr = INT32_MIN;
    for(int i = n - 1 ; i >= 0 ; i--){
        if(temp < 0) temp = array[i];
        else temp += array[i];
        maxCurr = max(maxCurr, temp);

        rhs[i] = maxCurr;
    }

    int ans = array[0], tempAns = INT32_MIN; temp = INT32_MIN, maxCurr = INT32_MIN;
    for(int i = 0 ; i < n - 1 ; i++){
        if(temp < 0) temp = array[i];
        else temp += array[i];
        maxCurr = max(maxCurr, temp);

        tempAns = maxCurr + rhs[i + 1];
        ans = max(tempAns, ans);
    }

    cout << ans;
    return 0;
}