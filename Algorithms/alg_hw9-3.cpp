#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstring>

using namespace std;

vector<vector<int>> adjlist(200010);
unordered_set<int> hashTable;
unordered_set<int> needFix;
vector<int> visited(200010), dist(200010);
int passDisrupt = 0, randomStart, nextStart, height = 0;

void dfs1(int u, int m){
    visited[u]++;
    height++;

    for (int v : adjlist[u])
        if (!visit[v]){
            dist[v] = height;
            dfs(v);
        } 
}

void dfs2(int u, int m){
    visited[u]++;
    height++;
    if(needFix.count(u) && (dist[nextStart] < dist[u])) nextStart = u;

    for (int v : adjlist[u])
        if (!visit[v]){
            dist[v] = height;
            dfs(v);
        }
}

int findMinCost(int n, int m){
    fill(visited.begin(), visited.end(), 0);
    dist[randomStart] = 0;
    dfs1(randomStart, m);
    nextStart = 0;
    for(int i = 0 ; i < n ; i++) 
        if(needFix.count(i) && (dist[nextStart] < dist[i]))
            nextStart = i;

    fill(visited.begin(), visited.end(), 0);
    dist[nextStart] = 0;
    dfs2(nextStart, m);
}

int main(){
    int n, m;
    cin >> n >> m;

    for(int i = 0 ; i < n - 1 ; i++){
        int u, v;
        cin >> u >> v;
        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }

    for(int i = 0 ; i < m ; i++){
        cin >> randomStart;
        needFix.insert(randomStart);
    }
    cout << findMinCost(n, m);
    return 0;
}