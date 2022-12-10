#include<stdio.h>

int main(){
    int cou = 0,i = 0;
    char c;
    while(1000){
        scanf("%c",&c);

        if(c == ' '){
            cou++;
            if(cou == 2){
                c = 'a';
                cou--;
            }else{
                printf("%c",c);
                i++;
            }
        }else if(c == '\n'){
			break;        
        }else{
            printf("%c",c);
            cou = 0;
            i++;
        }
    }
    return 0;
}
