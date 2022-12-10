#include<stdio.h>

int main(){
    int n , sum = 0, pro ;
    scanf("%d",&n);
    int temp = n;
    while(n>0){
    	int fac =  1;
        pro = n % 10;
        for(int i=1;i<=pro;i++){
        	fac = fac*i;
        }
        sum += fac;
        n = (int)n/10;
    }
    if(sum == temp){
        	printf("%d is a strong number.",temp);
        }else{
        	printf("%d is not a strong number.",temp);
        }
    return 0;
}
