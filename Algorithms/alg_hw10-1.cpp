#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void dfs(vector<vector<int>>& adjlist, vector<int>& visit, int u) {
    visit[u]++;
    for (int v : adjlist[u])
        if (!visit[v]) dfs(adjlist, visit, v);
}

void graphReverse(vector<vector<int>>& adjlist, vector<vector<int>>& adjlistRe, int n) {
    for (int i = 0; i < n; i++)
        for (int j : adjlist[i])
            adjlistRe[j].push_back(i);
}

void graph(vector<vector<int>>& adjlist, int n, int m) {
    for (int i = 0; i < m; i++) {
        int u, v, d;
        cin >> u >> v >> d;

        adjlist[u - 1].push_back(v - 1);
        if (d == 2)
            adjlist[v - 1].push_back(u - 1);
    }

    for (int i = 0; i < n; i++)
        if (adjlist[i].size())
            sort(adjlist[i].begin(), adjlist[i].end());
}

int isSCC(int n, int m) {
    vector<vector<int>> adjlist(n), adjlistRe(n);
    vector<int> visit(n);
    int u = 0;

    fill(visit.begin(), visit.end(), 0);
    graph(adjlist, n, m);
    dfs(adjlist, visit, u);
    if (find(visit.begin(), visit.end(), 0) != visit.end())
        return 0;

    fill(visit.begin(), visit.end(), 0);
    graphReverse(adjlist, adjlistRe, n);
    dfs(adjlistRe, visit, u);
    if (find(visit.begin(), visit.end(), 0) != visit.end())
        return 0;

    return 1;
}

int main() {
    while (true) {
        int n, m;
        cin >> n >> m;

        if (!n || !m) break;
        cout << isSCC(n, m) << endl;
    }

    return 0;
}