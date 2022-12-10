#include<stdio.h>

int main(){
    int n;
    scanf("%d",&n);
    int height = n , width = 2*n-1;
    for(int i=1;i<=height;i++){
    	for(int j=1;j<=height-i;j++){
        	printf(" ");
        }
        for(int k=1;k<=2*i-1;k++){
        	printf("*");
        }
        printf("\n");
    }
    return 0;
}
