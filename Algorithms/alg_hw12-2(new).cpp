#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int visited[501] = {0}, adj[501][501];

void topology(int n, int m){
    memset(visited, 0, sizeof(visited));
    memset(adj, 0, sizeof(adj));
    for(int i = 0 ; i < m ; i++){
        int dl, dr;
        cin >> dl >> dr;
        adj[dl - 1][dr - 1]++;
    }

    for(int i = 0 ; i < 0 ; i++)
        for(int j = 0 ; j < adj[i].size() ; j++)
            visited[adj[i][j]]++;
    
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