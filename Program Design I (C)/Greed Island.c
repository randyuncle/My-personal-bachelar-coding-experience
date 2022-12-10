#include<stdio.h>

int main(){
    char ch[20];
    for(int i=0;i<20;i++){
    	scanf("%c",&ch[i]);
    }
    for(int i=0;i<20;i++){
        if(ch[i] == 'A'){
        	printf("C");
        }else if(ch[i] == 'B'){
         	printf("R");   
        }else if(ch[i] == 'C'){
         	printf("W");   
        }else if(ch[i] == 'D'){
         	printf("Z");   
        }else if(ch[i] == 'E'){
         	printf("G");   
        }
    }
    return 0;
}
