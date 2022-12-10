#include <stdio.h>
long int B(int n);

int main(void){
    int n;
    scanf("%d",&n);
    printf("%ld",B(n));
    return 0;
}
long int B(int n){
    if( n<=2 ){
        return 1;
    }else{
        return (2*B(n-1) + 3*B(n-2)) % 1000000007;
    }
}
