#include<stdio.h>
#include<string.h>

void add(char a[], char b[], char res[]);

int main(){
    char a[100], b[100], res[105];
    scanf("%s %s", a, b);
    add(a, b, res);
    printf("%s", res);
    return 0;
}
void add(char a[], char b[], char res[]){
    int next = 0,len1,len2,n=0,p=0;
    char *an = &a[0] , *bn = &b[0];
    char temp[105];
    len1 = strlen(a) ;
    len2 = strlen(b) ;
    an += len1 - 1;
    bn += len2 - 1;
    while(1){
        int sum = next;
        if(len1>0){
            sum += *an - '0';
            len1--;
            an--;
        }

        if(len2>0){
            sum += *bn - '0';
            len2--;
            bn--;
        }

        temp[n] = sum % 10 + '0';
        n++;
        next = sum / 10;

        if(len1 == 0 && len2 == 0){
            break;
        }
    }

    if(next){
        temp[n] = next;
        n++;
    }

    p = n - 1;

    for(int r=0 ; r<n ; r++ , p--){
        res[p] = temp[r];
    }
    for(int r=n ; r<105 ; r++){
        res[r] = 0;
    }

}
