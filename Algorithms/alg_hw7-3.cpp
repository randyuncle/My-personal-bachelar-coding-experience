#include <iostream>
#include <cstring>
#define MAX 30001

using namespace std;

typedef struct {
    int s, lt, t;
}element;

element arr[20001], buffer[20001];

void merge(int front, int mid, int end) {
    int indL = front, indR = mid + 1, i = front;
    while ((indL <= mid) && (indR <= end)) {
        if (arr[indL].lt < arr[indR].lt) buffer[i++] = arr[indL++];
        else if (arr[indL].lt > arr[indR].lt) buffer[i++] = arr[indR++];
        else {
            if (arr[indL].t < arr[indR].t) buffer[i++] = arr[indL++];
            else buffer[i++] = arr[indR++];
        }
    }

    while (indL <= mid) buffer[i++] = arr[indL++];
    while (indR <= end) buffer[i++] = arr[indR++];

    for (i = front; i <= end; i++) arr[i] = buffer[i];
}
void mergeSort(int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort(mid + 1, end);
        merge(front, mid, end);
    }
}

int findMax(int n) {
    int dp[MAX], ans = 0;
    //initialize
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i < n; i++) {
        for (int j = arr[i].lt; j >= arr[i].t; j--) {
            dp[j] = max(dp[j], dp[j - arr[i].t] + arr[i].s);
            ans = max(dp[j], ans);
        }
    }

    return ans;
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) cin >> arr[i].s >> arr[i].lt >> arr[i].t;

    mergeSort(0, n - 1);
    cout << findMax(n);

    return 0;
}