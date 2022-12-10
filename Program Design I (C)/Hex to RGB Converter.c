#include<stdio.h>

int main(){
    char hex;
    int x=0,r=0,g=0,b=0;
    for(int i=0 ; i<=6 ; i++ ){
        scanf("%c",&hex);
        if(i>0){
            switch(hex){
                case 'A':
                    x = 10;
                    break;
                case 'B':
                    x = 11;
                    break;
                case 'C':
                    x = 12;
                    break;
                case 'D':
                    x = 13;
                    break;
                case 'E':
                    x = 14;
                    break;
                case 'F':
                    x = 15;
                    break;
                default :
                    x = hex - '0';
            }
            if(i == 1)
                r += x*16;
            else if(i == 2)
                r += x;
            else if(i == 3)
                g += x*16;
            else if(i == 4)
                g += x;
            else if(i == 5)
                b += x*16;
            else if(i == 6)
                b += x;
        }
    }
    printf("rgb(%d,%d,%d)",r,g,b);
    return 0;
}