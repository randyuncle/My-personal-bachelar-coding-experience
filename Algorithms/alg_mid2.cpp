#include <iostream>
#include <vector>
#include <algorithm>
#define MAX 100001

using namespace std;

vector <int> arr(MAX); 

void BS(int front, int end){
    vector <int> test, result(MAX);
    for (int i = end ; i >= 0; i--) {
        int left = 0, right = test.size();
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (test[mid] >= arr[i]) right = mid;
            else left = mid + 1;
        }
        result[i] = right;
        test.insert(test.begin() + right, arr[i]);
    }

    for(int i = 0 ; i < end ; i++) cout << result[i] << " ";
    cout << result[end];
}

int main(){
    int n;
    cin >> n;
    for(int i = 0 ; i < n ; i++) cin >> arr[i];

    BS(0, n - 1);
    return 0;
}