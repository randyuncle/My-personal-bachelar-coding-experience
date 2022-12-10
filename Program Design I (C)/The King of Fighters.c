#include<stdio.h>

int main(){
    int h1,h2,attA,attB;
    scanf("%d%d",&h1,&h2);
    while((h1>0)&&(h2>0)){
    	scanf("%d",&attA);
        h2 = h2 - attA;
        if(h2>0){
        	scanf("%d",&attB);
            h1 = h1 - attB;
        }else{
        	break;
        }
    }
    if(h1<=0){
    	printf("B");
    }else{
    	printf("A");
    }
    return 0;
}
