#include <iostream>
#include <queue>
#include <cstdbool>

using namespace std;

int n, m, sx, sy, tx, ty;
int maze[310][310], visited[310][310];
typedef struct element{
    int x;
    int y;
}element;
queue <element> q;

bool findPath(){
    if(sx == tx && sy == ty) return true;
    
    element start, end;
    start.x = sx - 1; start.y = sy - 1; end.x = tx - 1; end.y = ty - 1;

    int row[4] = {-1, 0, 1, 0};
    int col[4] = {0, 1, 0, -1};

    q.push(start);
    visited[start.x][start.y]++;
    while(q.size()){
        element curr = q.front();
        q.pop();
        for(int i = 0 ; i < 4 ; i++){
            int x = curr.x + row[i];
            int y = curr.y + col[i];
            
            if(x >= 0 && x < n && y >= 0 && y < m){
                if(maze[x][y] && !visited[x][y]){
                    visited[x][y]++;
                    if(x == end.x && y == end.y)
                        return true;
                    element next;
                    next.x = x; next.y = y;
                    q.push(next);
                }
            }
        }
    }
    return false;
}

int main(){
    cin >> n >> m >> sx >> sy >> tx >> ty;

    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++){
            cin >> maze[i][j];
            visited[i][j] = 0;
        }
    }
    

    if(findPath()) cout << "Success!";
    else cout << "Fail!";

    return 0;
}