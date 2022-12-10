#include<stdio.h>

int main(){
    int k,i = 0 ;
    char w;
    scanf("%d",&k);
    if(k>=26 || k<=-26){
        k = k % 26;
    }
    while(1){
        scanf("%c",&w);
        int temp = w;
        if('a'<=w && w<='z'){
            temp = temp + k;
            if(temp>'z'){
                temp = temp - 'z' + 'a' - 1;
            }else if(temp<'a'){
                temp = temp + 'z' - 'a' + 1;
            }
            printf("%c",temp);
        }else if('A'<=w && w<='Z') {
            temp = temp + k;
            if (temp > 'Z') {
                temp = temp - 'Z' + 'A' - 1;
            }else if(temp<'A'){
                temp = temp + 'Z' - 'A' + 1;
            }
            printf("%c",temp);
        }else if(w == '\n'){
            i++;
            if(i == 2){
                break;
            }
        }else{
            printf("%c", w);
        }
    }
    return 0;
}


