#include<stdio.h>

int main(){
    int x1,x2,n,i,j = 0,a[20],sum[10000];
    while(1){
        scanf("%d%d%d",&x1,&x2,&n);
        if(x1 == 0 && x2 == 0 && n == 0){
            break;
        }else{
            a[0] = x1;
            a[1] = x2;
            for(i=2;i<n;i++){
                a[i] = a[i-1] + a[i-2];
            }
            sum[j] = a[i-1];
            j++;
        }
    }
     for(i=0;i<j;i++)
            printf("%d\n",sum[i]);
    return 0;
}
