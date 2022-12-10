#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define MAX 1001

using namespace std;

int heap[MAX], listCount = 0, heapCou;
unordered_map <int, int> hashSave;
vector <int> lists[MAX];

void Swap(int smallest, int node) {
    int temp = heap[smallest];
    heap[smallest] = heap[node];
    heap[node] = temp;
    return;
}
void minHeapify(int node, int length)
{
    int left = 2 * node, right = 2 * node + 1, smallest;

    if (left <= length && heap[left] < heap[node]) smallest = left;
    else smallest = node;

    if (right <= length && heap[right] < heap[smallest]) smallest = right;

    if (smallest != node) {
        Swap(smallest, node);
        minHeapify(smallest, length);
    }
    return;
}

int deleteMin() {
    if (heapCou == 1) return heap[heapCou--];

    int min = heap[1];
    heap[1] = heap[heapCou];
    heap[heapCou--] = 0;
    minHeapify(1, heapCou);
    return min;
}

void listsMade(int n) {
    for (int i = 1; i <= n; i++) {
        int data = deleteMin();
        if (hashSave[data] > listCount)
            listCount = hashSave[data];
        lists[hashSave[data]--].push_back(data);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    heapCou = n;

    for (int i = 1; i <= n; i++) {
        cin >> heap[i];
        if (!hashSave.count(heap[i]))
            hashSave.insert(pair<int, int>(heap[i], 1));
        else
            hashSave[heap[i]]++;
    }

    for (int i = (int)n / 2; i >= 1; i--){
        minHeapify(i, n);
    }

    listsMade(n);

    for (int i = 1; i <= listCount; i++) {
        for (int j = 0; j < lists[i].size() ; j++) {
            cout << lists[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}