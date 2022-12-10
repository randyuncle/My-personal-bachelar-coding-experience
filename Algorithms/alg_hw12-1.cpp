#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;
int visited[1010] = {0}, adjmat[1010][1010];

int choose(int distance[], int n){
    int min = INT32_MAX, minpos = -1;
    for(int i = 0 ; i < n ; i++){
        if(distance[i] && distance[i] < min && !visited[i]){
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

int dijkestra(int n, int v){
    int distance[n + 1], ans = 0;
    for(int i = 0 ; i < n ; i++)
        distance[i] = adjmat[v][i];
    
    visited[v]++;
    distance[v] = 0;
    for(int i = 0 ; i < n - 2 ; i++){
        int u = choose(distance, n);
        visited[u]++;
        for(int w = 0 ; w < n ; w++) //greedy
            if(!visited[w] && w != u)
                if(distance[u] + adjmat[u][w] < distance[w])
                    distance[w] = distance[u] + adjmat[u][w];
    }
    
    for(int i = 0 ; i < n ; i++)
        if(distance[i] > ans) ans = distance[i];
    return ans;
}

int main(){
    int n;
    cin >> n;

    memset(adjmat, 0, sizeof(adjmat));
    for(int i = 1 ; i < n ; i++){
        for(int j = 0 ; j < i ; j++){
            int input;
            cin >> input;
            adjmat[i][j] = input;
            adjmat[j][i] = input;
        } 
    }
    
    cout << dijkestra(n, 0);
    return 0;
}