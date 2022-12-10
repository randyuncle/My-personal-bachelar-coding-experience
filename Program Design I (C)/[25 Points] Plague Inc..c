#include<stdio.h>

int main(){
    double r,p_d = 1.00;
    int n;
    scanf("%lf %d",&r,&n);
    for(int i=0 ; i<n ; i++ ){
    	p_d = p_d*r;
    }
    int p = p_d;
    printf("%d",p);
    return 0;
}
