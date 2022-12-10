#include<stdio.h>
int com( int n, int m);

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    printf("%d",com(n,m));
    return 0;
}
int com( int n, int m){
    if(m == n || m == 0){
        return 1;
    }else{
        return (com(n-1,m-1) + com(n-1,m));
    }
}
