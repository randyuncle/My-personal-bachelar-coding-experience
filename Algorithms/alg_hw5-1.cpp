#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>

using namespace std;

struct element {
    double con;
    string name;
};
struct element wine[500000], buffer[500000];

void merge(int front, int mid, int end) {
    int indL = front, indR = mid + 1, i = front;
    while ((indL <= mid) && (indR <= end)) {
        if (wine[indL].con < wine[indR].con) buffer[i++] = wine[indL++];
        else buffer[i++] = wine[indR++];
    }

    while (indL <= mid) buffer[i++] = wine[indL++];
    while (indR <= end) buffer[i++] = wine[indR++];

    for (i = front; i <= end; i++) wine[i] = buffer[i];
    return;
}
void mergeSort(int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort(mid + 1, end);
        merge(front, mid, end);
    }

    return;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;

    for (int i = 0; i < n; i++) {
        double a, b, c; string str;
        cin >> a >> b >> c >> str;
        double con = c / (a + b + c);
        wine[i].con = con;
        wine[i].name = str;
    }

    mergeSort(0, n - 1);

    cout << wine[n - k].name;
    return 0;
}