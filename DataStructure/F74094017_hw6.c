#include <stdio.h>
#include <string.h>
#define MAX 100


int topPos = -1;
int topPre = -1;

char prefix[MAX][MAX];
char postfix[MAX][MAX];

char buffer1[MAX];
char buffer2[MAX];

//postfrix to prefix
void posToPre(char b[])
{
    char curr[1];

    for(int i = 0 ; i < strlen(b) ; i++)
    {
        curr[0] = b[i];
        char strInsert[MAX];

        if(curr[0] == '+' || curr[0] == '-' || curr[0] == '*' || curr[0] == '/' || curr[0] == '^')
        {
            pop(1);
            strcpy(strInsert,curr);
            strcat(strInsert,buffer2); 
            strcat(strInsert,buffer1);            
            push(strInsert,1);
        }
        else
        {
            push(curr,1);
        }
    }

    for(int i = 0 ; i <= strlen(b) - 1 ; i++) 
    {
        printf("%c",prefix[0][2*i]);
    }
}

//prefix to postfix
void preToPos(char a[])
{
    char curr[1];
    for(int i = strlen(a) - 1 ; i >= 0 ; i--)
    {
        char strInsert[MAX];
        curr[0] = a[i];

        if(*curr == '+' || *curr == '-' || *curr == '*' || *curr == '/' || *curr == '^')
        {
            pop(2);
            strcpy(strInsert,buffer1);
            strcat(strInsert,buffer2);
            strcat(strInsert,curr);
            push(strInsert,2);
        }
        else
        {
            push(curr,2);
        }
    }

    //print it out
    for(int i = 0 ; i <= strlen(a) - 1 ; i++) 
    {
        printf("%c",postfix[0][2*i]);
    }
}

void push(char *str,int command)
{
    if(command == 1)
    {
        if(topPre > MAX)
        {
            return;
        }
        else
        {
            topPre += 1;
            strcpy(prefix[topPre],str);
        }
        
    }
    else
    {
        if(topPos > MAX)
        {
            return;
        }
        else
        {
            topPos += 1;
            strcpy(postfix[topPos],str);
        }
    }
    
}

void pop(int command)
{

    if(command == 1)
    {
        if(topPre == -1)
        {
            return;
        }
        else
        {
           strcpy(buffer1,prefix[topPre--]);
           strcpy(buffer2,prefix[topPre--]);
        }
    }
    else
    {
        if(topPos == -1)
        {
            return;
        }
        else
        {
            strcpy(buffer1,postfix[topPos--]);
            strcpy(buffer2,postfix[topPos--]);
        }
    }
}

int main()
{
    char a[MAX],b[MAX];

    scanf("%s",&a);
    scanf("%s",&b);

    preToPos(a);

    printf("\n");

    posToPre(b);

    return 0;
}