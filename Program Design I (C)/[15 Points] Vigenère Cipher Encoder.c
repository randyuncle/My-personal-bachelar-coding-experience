#include<stdio.h>

int main(){
    int l = 0,counter = 0,temp;
    scanf("%d",&l);
    int s[l];
    char c;
    for(int i=0 ; i<=l ; i++ ){
        scanf("%c",&c);
        s[i] = c - 'a';
    }
    int i = 1;
    while(1){
        scanf("%c",&c);
        temp = c;
        if(c == '\n'){
            counter++;
            if(counter == 2){
                break;
            }
        }else if(temp>='a' && temp<='z'){
            temp += s[i];
            if(temp > 'z'){
                temp = temp - 'z' + 'a' - 1;
                printf("%c",temp);
            }else{
                printf("%c",temp);
            }
            i++;
        }else if(c>='A' && c<='Z'){
            temp += s[i];
            if(temp > 'Z'){
                temp = temp - 'Z' + 'A' - 1;
                printf("%c",temp);
            }else{
                printf("%c",temp);
            }
            i++;
        }else{
            printf("%c",temp);
            i++;
        }
        if(i > l)
            i = 1;
    }
    return 0;
}
