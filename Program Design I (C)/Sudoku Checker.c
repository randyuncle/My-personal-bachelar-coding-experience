#include <stdio.h>
#define NUM 9

void check_sudoku(int (*grid_p)[NUM]);

int main(void){
    int grid[NUM][NUM]; // sudoku puzzle
    for(int i = 0; i < NUM; ++i){
        for(int j = 0; j < NUM; ++j){
            scanf("%d", &grid[i][j]);
        }
    }
    check_sudoku(grid);
    return 0;
}

void check_sudoku(int (*grid_p)[NUM]){
    int *stan = &grid_p[0][0], *p , a[9][9];
	
	for(int i=0 ; i<NUM ; i++){
		for(int j=0 ; j<NUM ; j++){
			a[i][j] = 1;
		}
	}

    //for row //debug complete
    for(int i=0 ; i<NUM ; i++){
        for(int colemn=0 ; colemn<NUM ; colemn++){
            for(p = stan + i*NUM ; p < stan + (i+1)*NUM ; p++ ){
                if((*(stan + i*NUM + colemn) == *p) && ((stan + i*NUM + colemn) != p)){
                    a[i][colemn] = 0;
                }
            }
        }
    }

    //for colemn //debug complete //another bug //debug complete_2
    for(int i=0 ; i<NUM ; i++){
        for(int row=0 ; row<NUM ; row++){
            for(p = stan +i ; p < stan + NUM*NUM ; p += NUM ){
                if((*(stan + row*NUM + i) == *p) && ((stan + row*NUM + i) != p)) {
                    a[row][i] = 0;
                }
            }
        }
    }

    //for block //6 for(? //debug complete
    for(int R=0 ; R<3 ; R++){//total 9*9 row
        for(int C=0 ; C<3 ; C++){//total 9*9 colemn
            for(int r=0 ; r<3 ; r++){//the testing 3*3 row
                for(int c=0 ; c<3 ; c++){//the testing 3*3 colemn
                    for(int row=0 + 3*R ; row< 3*(R+1) ; row++){
                        for(p=stan+9*row+3*C ; p<stan+9*row+3*(C+1) ; p++){
                            if((*(stan + 9*(r+3*R) + (c+3*C)) == *p) && (stan + 9*(r+3*R) + (c+3*C) != p)){
                            	a[r+3*R][c+3*C] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    //print
    for(int i=0 ; i<NUM ; i++){
		for(int j=0 ; j<NUM ; j++){
			if(a[i][j] == 0)
				printf("(%d,%d)\n",i,j);
		}
	}
}