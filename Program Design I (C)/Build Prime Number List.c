#include<stdio.h>

int main(){
    int x,y,cou = 0;
    scanf("%d%d",&x,&y);
    for(int i=x;i<=y;i++){
        if(i>1){
            
                for (int j = 2; j < i; j++) {
                    if (i % j == 0) {
                        cou++;
                        break;
                    }
                }
                if (cou == 0 ) {
                    printf("%d ", i);
                }
                cou = 0;
               
        }
    }
    return 0;
}
