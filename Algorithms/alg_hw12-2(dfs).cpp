#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int visited[501] = {0}, adj[501][501], order[501] = {0}, t = 0;

void dfs(int i, int n){
    if(visited[i]) return;

    visited[i]++;
    for(int j = 1 ; j <= n ; j++)
        if(adj[i][j]) 
            dfs(j, n);
    order[t++] = i;
}

void topology(int n, int m){
    t = 0;
    memset(order, 0, sizeof(order));
    memset(visited, 0, sizeof(visited));
    memset(adj, 0, sizeof(adj));
    for(int i = 0 ; i < m ; i++){
        int dl, dr;
        cin >> dl >> dr;
        adj[dl][dr]++;
    }

    for(int i = 1 ; i <= n ; i++)
        if(!visited[i]) dfs(i, n);
    
    for(int i = n ; i >= 1 ; i--)
        cout << order[i] << " ";
    cout << endl;
}

int main(){
    int n, m;
    while(cin >> n >> m)
        topology(n, m);
    return 0;
}