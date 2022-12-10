#include<stdio.h>

int main(){
    int p1,p2,p3,a,b,c,d;
    scanf("%d%d%d%d",&a,&b,&c,&d);
    p1 = a;
    p2 = p1-b+c;
    p3 = p2-d;
    printf("%04d %04d %04d",p1,p2,p3);
    return 0;
}

