#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int high = 0;
int length = 0; 

typedef struct
{
    int h;
    int l;
}
node;

node queue[2500];
int rear = -1;
int front = -1;
int size = 0;

void enqueue(int h,int l)
{
    rear = (rear + 1) % 2500;

    queue[rear].h = h;
    queue[rear].l = l;

    size++;
}

bool dequeue()
{
    if(front == rear)
    {
        return false;
    }

    rear = (rear - 1) % 2500;

    return true;
}



bool findPath(int maze[][50],int visited[][50],int currH,int currL)
{
    int *p1 = &maze[currH][currL], *p2 = &visited[currH][currL];
    int h = currH,l = currL;

    // *p2 == 1 -> also be known as visited 
    if(l >= 0 && l < length && h >= 0 && h < high && *p1 == 0 && *p2 == 0)
    {
        if(l == (length - 1) && h == (high - 1))
        {
            enqueue(h,l);
            *p2 = 1;

            return true;
        }

        *p2 = 1;
        enqueue(h,l);

        if(findPath(maze,visited,h+1,l))
        {
            return true;
        }
        if(findPath(maze,visited,h-1,l))
        {
            return true;
        }
        if(findPath(maze,visited,h+1,l+1))
        {
            return true;
        }
        if(findPath(maze,visited,h+1,l-1))
        {
            return true;
        }
        if(findPath(maze,visited,h-1,l+1))
        {
            return true;
        }
        if(findPath(maze,visited,h-1,l-1))
        {
            return true;
        }
        if(findPath(maze,visited,h,l+1))
        {
            return true;
        }
        if(findPath(maze,visited,h,l-1))
        {
            return true;
        }

        *p2 = 0;
        dequeue();

        return false;
    }
    return false;
}

int main()
{
    int maze[50][50],visited[50][50];

    scanf("%d %d",&high,&length);

    for(int h = 0 ; h < high ; h++)
    {
        for(int l = 0 ; l < length ; l++)
        {
            scanf("%d",&maze[h][l]);
            visited[h][l] = 0;
        }
    }

    if(findPath(maze,visited,0,0))
    {
        for(int i = (front + 1) ; i <= rear ; i++)
        {
            printf("%d %d\n",queue[i].h,queue[i].l);
        }
    }
    else
    {
        printf("no answer");
    }

    return 0;
}