#include <stdio.h>
#include <stdbool.h>
#define n 5

bool visit(char(*)[n], int(*)[n], int, int);
int main(void) {
    char maze[n][n];
    int route[n][n];
    // initialize maze and route matrices
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
	    route[i][j]=0;
	    scanf("%c", &maze[i][j]);
	    getchar();
	}
    }
    if (visit(maze, route, 0, 0)) { // (0,0) is a starting point
        for (int i=0; i<n; i++) {
	    for (int j=0; j<n; j++)
	        printf("%d ", route[i][j]);
	    printf("\n");
        }
    } else {
	printf("Can't find the exit!");
    }
    return 0;
}// TODO: fill the route matrix with 0's or 1‘s.
// If there isn't any route, return false; otherwise, return true.
bool visit(char maze[][n], int route[][n], int originX, int originY) {
    char *p1 = &maze[originX][originY];
    int *p2 = &route[originX][originY],x=originX,y=originY;
    if( x < n && y < n && x>=0 && y>=0 && *p2 == 0 &&*p1 == 'r'){
        if(x == 4 && y == 4){
            *p2 = 1;
            return true;
        }
        *p2 = 1;
        if(visit(maze, route, x+1, y) == true){
            return true;
        }
        if(visit(maze, route, x, y+1) == true){
            return true;
        }
        if(visit(maze, route, x-1, y) == true){
            return true;
        }
        if(visit(maze, route, x, y-1) == true){
            return true;
        }
        *p2 = 0;
        return false;
    }
    return false;
}

