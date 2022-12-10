#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct element {
    int from, p;
}element;
vector<vector<int>> adjlist(200000);
int visit[200000] = { 0 }, low[200000] = { 0 }, discovery[200000] = { 0 };
element priority[200000], buffer[200000];

void merge(int front, int mid, int end) {
    int indL = front, indR = mid + 1, i = front;
    while ((indL <= mid) && (indR <= end)) {
        if (priority[indL].p > priority[indR].p) buffer[i++] = priority[indL++];
        else if (priority[indL].p < priority[indR].p) buffer[i++] = priority[indR++];
        else {
            if (priority[indL].from < priority[indR].from) buffer[i++] = priority[indL++];
            else buffer[i++] = priority[indR++];
        }
    }

    while (indL <= mid) buffer[i++] = priority[indL++];
    while (indR <= end) buffer[i++] = priority[indR++];

    for (i = front; i <= end; i++) priority[i] = buffer[i];
}
void mergeSort(int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort(mid + 1, end);
        merge(front, mid, end);
    }
}

void dfs(int u, int up, int &time) {
    int child = 0;
    discovery[u] = low[u] = time++;

    visit[u]++;
    for (int v : adjlist[u]) {
        if (!visit[v]) {
            child++;
            dfs(v, u, time);

            low[u] = min(low[u], low[v]);
            if (up != -1 && low[v] >= discovery[u])
                priority[u].p++;
            else if (up == -1 && child > 1)
                priority[u].p++;
        }
        else if (v != up)
            low[u] = min(low[u], discovery[v]);
    }
}

void findAP(int n, int m) {
    int time = 0;
    dfs(0, -1, time);
    mergeSort(0, n - 1);
    for (int i = 0; i < m; i++)
        cout << priority[i].from << " " << priority[i].p << endl;
}

int main() {
    int n, m;
    cin >> n >> m;

    while (true) {
        int u, v;
        cin >> u >> v;

        if (u ==  -1 && v == -1) break;

        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }

    for (int i = 0; i < n; i++) {
        sort(adjlist[i].begin(), adjlist[i].end());
        priority[i].from = i;
        priority[i].p = 1;
    }

    findAP(n, m);

    return 0;
}