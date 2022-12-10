#include<stdio.h>

int main(){
    unsigned long long int value = 0,fac_m = 1,fac_n = 1,mi = 0;
    int m,n;
    scanf("%d%d",&m,&n);
    if(m == n || n == 0){
        value = 1;
    }else{
        if((m-n)>n){
			mi = m - n + 1;
		}else if((m-n) == n){
            mi = n + 1;
        }else{
			mi = n + 1;
			n = m - n;
		}
        for(int i=mi ; i<=m ; i++ ){
            fac_m = fac_m*i;
        }
        for(int i=1 ; i<=n ; i++ ){
            fac_n = fac_n*i;
        }
        value = fac_m/fac_n;
    }

    printf("%llu",value);

    return 0;
}