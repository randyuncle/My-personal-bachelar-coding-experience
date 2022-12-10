#include<stdio.h>
#include<string.h>
//顧客抵達八樓的順序
int main(){
    int m,n,temp;
    scanf("%d %d",&m,&n);
    int a[(m+n)];
    for(int i=0 ; i<m ; i++ ){
        scanf("%d",a[i]);
    }
    for(int i=m-1 ; i<(m+n) ; i++ ){
        scanf("%d",&a[i]);
    }
    for(int i=0 ; i<(m+n) ; i++){
        for(int j=i+1 ; j<(m+n); j++ ){
			if(a[i]<a[j]){
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
    }
	printf("%s",a[(m+n)]);
    
    return 0;
}