#include <stdio.h>
#include <stdlib.h> 
#define MAX 100

int adj[MAX][MAX];
int visited[MAX];

//print的東西
int print[MAX][MAX];
int totalNode[MAX];
int nodes = 0;
int numOfPrint = 0;

//DFS
void DFS(int v, int t);

//Stack for the adaj. dfs traversal
int stack[10000000];
int top = -1;
void push(int v);
int pop();

void DFS(int v, int m)
{
    int i;
    push(v);

    while(top != -1)
    {
        v = pop();

        if(visited[v] == 0)
        {
            print[numOfPrint][nodes++] = v;
            visited[v]++;
        }

        for(i = m - 1 ; i >= 0; i--)
        {
            if(adj[v][i] == 1 && visited[i] == 0)
            {
                push(i);
            }       
        }
    }
}

void push(int v)
{
    if(top < 10000000)
    {
        stack[++top] = v;
    }
}

int pop()
{
    if(top > -1)
    {
        int v = stack[top--];
        return v;
    }

    return -1;
}

int main()
{
    int round;

    scanf("%d",&round);

    for(int i = 0 ; i < round ; i++)
    {
        int m,t;
        scanf("%d %d",&m,&t);
        totalNode[i] = m;

        for(int x = 0 ; x < m ; x++)
        {
            for(int y = 0 ; y < m ; y++)
            {
                scanf("%d",&adj[x][y]);
            }
        }

        for(int j = 0 ; j < m ; j++)
        {
            visited[j] = 0;
        }

        DFS(t,m);

        nodes = 0;
        numOfPrint++;
    }

    for(int i = 0 ; i < round ; i++)
    {
        for(int j = 0 ; j < totalNode[i] ; j++)
        {
            printf("%d ",print[i][j]);
        }

        printf("\n");
    }
    
    return 0;
}