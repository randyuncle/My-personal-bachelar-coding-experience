#include<stdio.h>

typedef struct
{
    int row;
    int col;
    int value;
}matrix;

matrix m[100];
matrix t[100];

void fastTranspose(int c,int turn)
{
    // rowTerms/startingPos
    int transMethod[turn][2];

    if(m[0].value > 0)
    {
        for(int i = 0 ; i < c ; i++)
        {
            transMethod[i][0] = 0;
        }

        //calculate row terms
        for(int i = 0 ; i < turn ; i++)
        {
            transMethod[m[i].col][0]++;
        }

        //doing transpose
        transMethod[0][1] = 0;

        for(int i = 1 ; i < c ; i++)
        {
            transMethod[i][1] = transMethod[i-1][1] + transMethod[i-1][0];
        }

        for(int i = 0 ; i < turn ; i++)
        {
            int newIndex = transMethod[m[i].col][1];

            t[newIndex].row = m[i].col;
            t[newIndex].col = m[i].row;
            t[newIndex].value = m[i].value;

            transMethod[m[i].col][1]++;
        }
    } 
}

int main()
{
    int r,c,turn;

    scanf("%d %d %d",&r,&c,&turn);

    for(int i = 0 ; i < turn ; i++)
    {
        scanf("%d %d %d",&m[i].row,&m[i].col,&m[i].value);
    }

    fastTranspose(c,turn);

    printf("%d %d %d\n",r,c,turn);

    for(int i = 0 ; i < turn ; i++)
    {
        printf("%d %d %d\n",t[i].row,t[i].col,t[i].value);
    }

    return 0;
}