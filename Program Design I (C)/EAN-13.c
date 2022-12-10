#include<stdio.h>

int main(){
    int a,b,c,d,e,f,g,h,i,j,k,l,sum,ckd;
    scanf("%1d",&a);
    scanf("%1d%1d%1d%1d%1d%1d",&b,&c,&d,&e,&f,&g);
    scanf("%1d%1d%1d%1d%1d",&h,&i,&j,&k,&l);
    sum = (a+c+e+g+i+k) + 3*(b+d+f+h+j+l);
    ckd = ((sum - 1) / 10 * 10 + 10) - sum ;
    printf("%d",ckd);
    return 0;
}
