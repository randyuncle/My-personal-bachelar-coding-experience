#include<stdio.h>

int main(){
    int wind;
    scanf("%d",&wind);
    if(wind<1){
    	printf("Calm");
    }else if(wind<=3){
    	printf("Light air");
    }else if(wind<=27){
    	printf("Breeze");
    }else if(wind<=47){
    	printf("Gale");
    }else if(wind<=63){
    	printf("Storm");
    }else{
    	printf("Hurricane");
    }
    return 0;
}
