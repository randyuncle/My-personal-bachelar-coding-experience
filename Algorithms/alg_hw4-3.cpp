#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#define MAX 200000

using namespace std;

pair <int, int> wine[MAX], buffer[MAX];

void merge(int front, int mid, int end) {
    int indL = front, indR = mid + 1, i = front;
    while ((indL <= mid) && (indR <= end)) {
        if (wine[indL].first < wine[indR].first) buffer[i++] = wine[indL++];
        else buffer[i++] = wine[indR++];
    }

    while (indL <= mid) buffer[i++] = wine[indL++];
    while (indR <= end) buffer[i++] = wine[indR++];

    for (i = front; i <= end; i++) wine[i] = buffer[i];
}
void mergeSort(int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort(mid + 1, end);
        merge(front, mid, end);
    }
}

int binarySearch(int front, int end, int seat){
    int mid = (front + end) / 2;
    while (front <= end) {
        int mid = (front + end) / 2;
        if (wine[mid].first > seat) 
            end = mid - 1;
        else if (wine[mid].first < seat) 
            front = mid + 1;
        else 
            return mid;        
    }
    return front;
}

void findWine(int n, int seat, int con) {
    int findSeat = binarySearch(0, n - 1, seat);
    int left = 0, right = 0, isFind = 0, ansSeat;

    if (wine[findSeat].second < con) {
        while((findSeat - left) >= 0) {
            if (wine[findSeat - left].second >= con) {
                isFind++;
                break;
            }
            left++;
        }

        while ((findSeat + right) < n) {
            if (wine[findSeat + right].second >= con) {
                isFind += 2;
                break;
            }
            right++;
        }

        if (isFind == 3) {
            int l = abs(seat - wine[findSeat - left].first), r = abs(wine[findSeat + right].first - seat);
            if (l > r)
                ansSeat = findSeat + right;
            else
                ansSeat = findSeat - left;
        }
        else if (isFind == 2) ansSeat = findSeat + right;
        else if (isFind == 1) ansSeat = findSeat - left;;
    }
    else {
        if(wine[findSeat].first == seat)
            ansSeat = findSeat;
        else if(findSeat + 1 < n){
            if(wine[findSeat + 1].second >= con && (abs(wine[findSeat + 1].first - seat) < abs(seat - wine[findSeat].first)))
                ansSeat = findSeat + 1;
        }
        else
            ansSeat = findSeat;
        isFind++;
    }

    if (isFind > 0) cout << wine[ansSeat].first << endl;
    else cout << -1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    for (int i = 0; i < n; i++) {
        int place, con;
        cin >> place >> con;
        wine[i].first = place;
        wine[i].second = con;
    }

    mergeSort(0, n - 1);

    for (int i = 0; i < q; i++) {
        int seat, con;
        cin >> seat >> con;

        findWine(n, seat, con);
    }

    return 0;
}