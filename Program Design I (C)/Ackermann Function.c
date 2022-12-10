#include <stdio.h>
int A(int n,int m);

int main(void){
    int n,m;
    scanf("%d %d",&n,&m);
    printf("%d",A(n,m));
	return 0;
}

int A(int n,int m){
	if(n==0)
        return (m + 1);
    else if(m == 0)
        A(n-1,1);
    else
        A(n-1,A(n,m-1));
}
