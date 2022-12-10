#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define N 100

int addition(char str[]);

int main() {
    char str[N];
    scanf("%s", str);
    int sum = addition(str);
    printf("%d", sum);
    return 0;
}int addition(char str[]){
    char *num[100];
    int i = 0,sum = 0;
    
    num[i] = strtok(str,"+");
    while(num[i] != NULL){
        i++;
    	num[i] = strtok(NULL,"+");
    }
    for(int j=0 ; j<i ; j++){
    	sum += atoi(num[j]);
    }
    return sum;
}