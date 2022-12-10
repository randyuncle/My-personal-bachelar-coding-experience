#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rising_hope(char *R);

int main(int argc, char *argv[])
{
    char in[1005];

    scanf("%s", in);
    int hope = rising_hope(in);
    printf("%d\n", hope);

    return 0;
}
int rising_hope(char *R) {
	int len,hp=0;
    char *p ;
    p = R;
    len = strlen(R);
    for(int i=0 ; i<len ; i++){
        if(*p == 'P' || *p == 'p'){
        	hp++;
        }else if(*p == 'N' || *p == 'n'){
        	hp--;
        }
        p++;
    }
    return hp;
}
