#include<stdio.h>

int main(){
    int n,x,a,b,p;
    float ck;
    scanf("%d%d%d",&n,&x,&a);
    b = a + x;
    if(b>=n){
        p = b / n;
    	b = b - n*p;
    }
    printf("%d",b);
    return 0;
}
