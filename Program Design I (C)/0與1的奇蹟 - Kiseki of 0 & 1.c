#include<stdio.h>

int main(){
    int a[32],b[32],c[32],pro = 0;
    for(int i=0 ; i<32 ; i++ )
        scanf("%d",&a[i]);
    for(int i=0 ; i<32 ; i++ ){
        scanf("%d",&b[i]);
        c[i] = a[i] + b[i];

    }
    for(int i=31 ; i>=0 ; i-- ){
        if(c[i]>1){
            pro = (int)(c[i] / 2);
            c[i] = c[i] % 2;
            if(i != 0)
                c[i-1] += 1*pro;
        }
    }
    for(int i=0 ; i<32 ; i++ )
        printf("%d ",c[i]);

    return 0;
}
