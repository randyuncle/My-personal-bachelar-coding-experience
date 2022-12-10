#include <stdio.h>
#include <stdlib.h> 
#define MAX 100

int adj[MAX][MAX];
int visited[MAX];

//print的東西
int print[MAX][MAX];
int node = 0;
int numOfPrint = 0;

//queue link list
typedef struct node *queuePointer;
typedef struct node
{
    int vertex;
    queuePointer next;
};

//the link list is construct by rear and front
queuePointer rear, front;

//conflicting type define
int deletq();
void addq(int vertex);

queuePointer createNode(int vertex)
{
    queuePointer node = (queuePointer)malloc(sizeof(struct node));

    node->vertex = vertex;
    node->next = NULL;

    return node;
}

void BFS(int v, int m)
{
    front = NULL;
    rear = NULL;

    print[numOfPrint][node++] = v;
    visited[v]++;
    addq(v);

    while(v != -1)
    {
        v = deletq();

        for(int i = 0 ; i < m && v != -1 ; i++)
        {
            if(adj[v][i] == 1 && visited[i] == 0)
            {
                print[numOfPrint][node++] = i;
                addq(i);
                visited[i]++;
            }
        }
    }
}

//加入佇列
void addq(int vertex)
{
    if(front == NULL)
    {
        front = createNode(vertex);
    }
    else
    {
        front->next = createNode(vertex);
        front = front->next;
    }
}

//rear前進，無法前進回傳-1結束迴圈
int deletq()
{
    if(front != rear)
    {
        if(rear == NULL)
        {
            rear = front;
            return rear->vertex;
        }

        rear = rear->next;
        return rear->vertex;
    }

    return -1;
}

int main()
{
    int round,totalNode[MAX];

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

        BFS(t,m);

        node = 0;
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