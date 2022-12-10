#include<stdio.h>

int i = 0;

void hanoi(int n,int st,int buf,int en)
{
    if(n > 0)
    {
        //1~(n-1)從A->B
        hanoi(n-1,st,en,buf);

        printf("%d %d %d\n",n,st,en);
        i++;

        //1~(n-1)從B->C
        hanoi(n-1,buf,st,en);
    }
}

int main()
{
    int n;

    scanf("%d",&n);

    hanoi(n,1,2,3);
    return 0;
}