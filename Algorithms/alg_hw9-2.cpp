#include <iostream>
#include <queue>
#include <cstdbool>

using namespace std;

int n, m, sx, sy, tx, ty;
int maze[1010][1010], visited[1010][1010], dist[1010][1010];
typedef struct element{
    int x;
    int y;
}element;
queue <element> q;

int findPath(){
    int step = 0;
    if(sx == tx && sy == ty) return step;
    
    element start, end;
    start.x = sx - 1; start.y = sy - 1; end.x = tx - 1; end.y = ty - 1;

    int row[4] = {-1, 0, 1, 0};
    int col[4] = {0, 1, 0, -1};

    q.push(start);
    visited[start.x][start.y]++;
    dist[start.x][start.y] = 0;
    while(q.size()){
        step++;
        element curr = q.front();
        q.pop();
        for(int i = 0 ; i < 4 ; i++){
            int x = curr.x + row[i];
            int y = curr.y + col[i];
            
            if(x >= 0 && x < n && y >= 0 && y < m){
                if(maze[x][y] && !visited[x][y]){
                    visited[x][y]++;
                    dist[x][y] = dist[curr.x][curr.y] + 1;
                    
                    if(x == end.x && y == end.y)
                        return dist[x][y];
                    
                    element next;
                    next.x = x; next.y = y;
                    q.push(next);
                }
            }
        }
    }
    return -1;
}

int main(){
    cin >> n >> m >> sx >> sy >> tx >> ty;

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++){
            cin >> maze[i][j];
            visited[i][j] = 0;
        }
    }

    cout << findPath();

    return 0;
}