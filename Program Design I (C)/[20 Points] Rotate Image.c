#include<stdio.h>

int main(){
    int degree,m,n;
    scanf("%d",&degree);
    scanf("%d %d",&m,&n);
    int a[m][n],ch1[m][n],ch2[n][m];
    for(int i=0 ; i<m ; i++ ){
        for(int j=0 ; j<n ; j++ ){
            scanf("%d",&a[i][j]);
        }
    }
    switch(degree){
        case 90:
            for(int i=0 ; i<n ; i++ ){
                for(int j=0 ; j<m ; j++ ){
                    printf("%d ",a[m-j-1][i]);
                }
                printf("\n");
            }
            break;
        case 180:
            for(int i=0 ; i<m ; i++ ) {
                for (int j = 0; j < n; j++) {
                    printf("%d ",a[m-i-1][n-j-1]);
                }
                printf("\n");
            }
            break;
        case 270:
            for(int i=0 ; i<n ; i++ ) {
                for (int j = 0; j < m; j++) {
                    printf("%d ",a[j][n-i-1]);
                }
                printf("\n");
            }
            break;    
    }
    return 0;
}
