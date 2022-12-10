#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

void topology(int n, int m){
    vector<int> adj[501];
    int visited[501] = {0};
    for(int i = 0 ; i < m ; i++){
        int dl, dr;
        cin >> dl >> dr;
        adj[dl].push_back(dr);
    }

    for(int i = 1 ; i <= n ; i++)
        for(int j = 0 ; j < adj[i].size() ; j++)
            visited[adj[i][j]]++;

    priority_queue<int, vector<int>, greater<int>> pq;
    for(int i = 1 ; i <= n ; i++)
        if(!visited[i]) pq.push(i);
    
    for(int i = 1 ; i <= n ; i++){
        int curr = pq.top();
        pq.pop();
        visited[curr] = -1;
        cout << curr << " ";

        for(int j = 0 ; j < adj[curr].size() ; j++){
            int next = adj[curr][j];
            visited[next]--;
            if(!visited[next]) pq.push(next);
        }
    }
    cout << endl;
}

int main(){
    int n, m, count = 0;
    while(cin >> n >> m)
        topology(n, m);
    return 0;
}