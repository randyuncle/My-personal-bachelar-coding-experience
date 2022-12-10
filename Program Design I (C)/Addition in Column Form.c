#include<stdio.h>

int main(){
    int x,y;
    scanf("%d%d",&x,&y);
    printf("  %04d\n+)%04d\n------\n%06d",x,y,x+y);
    return 0;
}
