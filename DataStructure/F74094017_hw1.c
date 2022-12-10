#include<stdio.h>
#include<string.h>

int recursive(int num)
{
    if(num == 0)
    {
        return 0;
    }
    else if(num == 1)
    {
        return 1; 
    }
    else
    {
        return (recursive(num - 1) + recursive(num - 2));
    }
}

int iterative(int num)
{
    int f[40];

    f[0] = 0;
    f[1] = 1;

    if(num == 0)
    {
        return 0;
    }
    else if(num == 1)
    {
        return 1; 
    }

    for(int i = 2 ; i <= num ; i++)
    {
        f[i] = f[i-1] + f[i-2];
    }

    return f[num];
}

int main()
{
    int n = 0,input = 0;
    char command[10][9];
    int ans[10];

    scanf("%d",&n);

    for(int i = 0 ; i < n ; i++)
    {
        scanf("%s %d",command[i],&input);
        if(strcmp(command[i],"recursive"))
        {
            ans[i] = recursive(input);
        }
        else if(strcmp(command[i],"iterative"))
        {
            ans[i] = iterative(input);
        }
    }

    for(int i = 0 ; i < n ; i++)
    {
        printf("%d\n",ans[i]);
    }

    return 0;
}