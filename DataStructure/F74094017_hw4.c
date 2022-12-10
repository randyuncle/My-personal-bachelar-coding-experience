#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 50000

int rowMove[8] = {-1,0,1,1,1,0,-1,-1};
int colMove[8] = {1,1,1,0,-1,-1,-1,0};

void a(int n, int m)
{
    int totalNum = n*m , visitedNum = 1 , cockroachRun = 1;
    int row = 0 , col = 0;
    int visited[n][m];

    srand((unsigned)time(NULL));

    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < m ; j++)
        {
            visited[i][j] = 0;
        }
    }

    visited[0][0] = 1;

    while(visitedNum < totalNum)
    {
        if(cockroachRun > MAX)
        {
            break;
        }

        int move = rand() % 8;


        if((row + rowMove[move]) >= n || (col + colMove[move]) >= m || (row + rowMove[move]) < 0 || (col + colMove[move]) < 0)
        {
            continue;
        }
        else
        {
            row += rowMove[move];
            col += colMove[move];

            if(visited[row][col] == 0)
            {
                visitedNum++;
            }

            visited[row][col]++;
            cockroachRun++;
        }
    }

    printf("%d\n",cockroachRun);

    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < m ; j++)
        {
            printf("%4d",visited[i][j]);
        }

        printf("\n");
    } 
}

void b(int n)
{
    int visitedNum = 1 , cockroachRun = 1;

    if(n == 1)
    {
        int totalNum = 15*15;
        int row = 10 , col = 10;
        int visited[15][15];

        srand((unsigned)time(NULL));

        for(int i = 0 ; i < 15 ; i++)
        {
            for(int j = 0 ; j < 15 ; j++)
            {
                visited[i][j] = 0;
            }
        }

        visited[10][10] = 1;

        while(visitedNum < totalNum)
        {
            if(cockroachRun > MAX)
            {
                break;
            }
            int move = rand() % 8;

            if((row + rowMove[move]) >= 15 || (col + colMove[move]) >= 15 || (row + rowMove[move]) < 0 || (col + colMove[move]) < 0)
            {
                continue;
            }
            else
            {
                row += rowMove[move];
                col += colMove[move];

                if(visited[row][col] == 0)
                {
                    visitedNum++;
                }

                visited[row][col]++;
                cockroachRun++;
            }
        }

        printf("%d\n",cockroachRun);

        for(int i = 0 ; i < 15; i++)
        {
            for(int j = 0 ; j < 15 ; j++)
            {
                printf("%4d",visited[i][j]);
            }

            printf("\n");
        }
    }
    else if(n == 2)
    {
        int totalNum = 39*19;
        int row = 1 , col = 1;
        int visited[39][19];

        for(int i = 0 ; i < 39 ; i++)
        {
            for(int j = 0 ; j < 19 ; j++)
            {
                visited[i][j] = 0;
            }
        }

        visited[1][1] = 1;

        while(visitedNum < totalNum)
        {
            if(cockroachRun > MAX)
            {
                break;
            }

            int move = rand() % 8;

            if((row + rowMove[move]) >= 39 || (col + colMove[move]) >= 19 || (row + rowMove[move]) < 0 || (col + colMove[move]) < 0)
            {
                continue;
            }
            else
            {
                row += rowMove[move];
                col += colMove[move];

                if(visited[row][col] == 0)
                {
                    visitedNum++;
                }

                visited[row][col]++;
                cockroachRun++;
            }
        }

        printf("%d\n",cockroachRun);

        for(int i = 0 ; i < 39; i++)
        {
            for(int j = 0 ; j < 19 ; j++)
            {
                printf("%4d",visited[i][j]);
            }

            printf("\n");
        }
    }
}

int main()
{
    char command[100];
    int counter = 0,n[100],m[100];

    scanf("%c",&command[counter]);

    while(command[counter] != 'q')
    {
        switch (command[counter])
        {
        case 'a':
            scanf("%d %d\n",&n[counter],&m[counter]);

            break;
        case 'b':
            scanf("%d\n",&n[counter]);

            break;
        default:
            break;
        }

        counter++;
        scanf("%c",&command[counter]);
    }

    for(int i = 0 ; i < counter ; i++)
    {
        if(command[i] == 'a')
        {
            a(n[i],m[i]);
        }
        else if(command[i] == 'b')
        {
            b(n[i]);
        }
    }

    return 0;
}