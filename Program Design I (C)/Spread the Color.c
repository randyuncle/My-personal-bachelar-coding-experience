#include <stdio.h>
#define M 5
#define N 8
char colors[4] = {'R', 'G', 'B', 'X'};   // Red, Green, Blue, Empty

void spread(char*, int, int);

int main(void) {
    char graph[M][N];
    int row, col;

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++)
            scanf("%c", &graph[i][j]);
        getchar();      // Ignore '\n'
    }
    scanf("%d %d", &row, &col);    // Starting position
    
    spread(&graph[0][0], row, col);
    
    // Print out the spreading result
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++)
            printf("%c", graph[i][j]);
        printf("\n");
    }
    return 0;
}void spread(char* graph, int row, int col) {
    char *p = &graph[0];
	//up
    for(int i = row - 1 ; i >= 0 ; i-- ){
    	if(*(p + i*N +col) == 'X'){
        	*(p + i*N +col) = *(p+row*N+col);
        }else if(*(p + i*N +col) == *(p+row*N+col) ){
        	continue;
        }else{
        	break;
        }   
    }
    //down
    for(int i = row + 1 ; i < M ; i++ ){
    	if(*(p + i*N +col) == 'X'){
        	*(p + i*N +col) = *(p+row*N+col);
        }else if(*(p + i*N +col) == *(p+row*N+col) ){
        	continue;
        }else{
        	break;
        }
    }
    //left
    for(int i = col - 1 ; i >= 0 ; i-- ){
    	if(*(p+row*N+i) == 'X'){
        	*(p+row*N+i) = *(p+row*N+col);
        }else if(*(p+row*N+i) == *(p+row*N+col) ){
        	continue;
        }else{
        	break;
        }
    }
    //right
    for(int i = col + 1 ; i < N ; i++ ){
    	if(*(p+row*N+i) == 'X'){
        	*(p+row*N+i) = *(p+row*N+col);
        }else if(*(p+row*N+i) == *(p+row*N+col) ){
        	continue;
        }else{
        	break;
        }
    }
}