#include<stdio.h>

int main(){
    int H,M;
    scanf("%d%d",&H,&M);
    M = M + 50;
    if(M>=60){
    	H = H + 1;
        M = M - 60;
    }
    printf("%02d:%02d\n",H,M);
    return 0;
}
