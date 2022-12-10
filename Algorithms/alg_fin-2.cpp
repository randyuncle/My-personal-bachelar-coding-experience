#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int canStop = 0;

void dfs(vector<vector<int>>& adjlist, vector<int>& visit, int u, int end) {
    visit[u]++;
    if(u == end){
        canStop++;
        return;
    } 
    for (int v : adjlist[u])
        if (!visit[v] && !canStop) dfs(adjlist, visit, v, end);
}

void trapFind(vector<int>& visit, int k){
    for(int i = 0 ; i < k ; i++){
        int trap;
        cin >> trap;
        visit[trap - 1]++;
    }
}

void graph(vector<vector<int>>& adjlist, int n, int m) {
    for (int i = 0; i < m; i++) {
        int u, v, d;
        cin >> u >> v;
        adjlist[u - 1].push_back(v - 1);
    }
}

int findPath(int n, int m, int k) {
    vector<vector<int>> adjlist(n);
    vector<int> visit(n);
    int start, end;

    cin >> start >> end;
    fill(visit.begin(), visit.end(), 0);
    graph(adjlist, n, m);
    trapFind(visit, k);
    if(visit[end - 1] || visit[start - 1]) return 0;

    dfs(adjlist, visit, start - 1, end - 1);
    if (!visit[end - 1])
        return 0;
    return 1;
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    if(findPath(n, m, k)) cout << "SAFE";
    else cout << "UNSAFE";

    return 0;
}