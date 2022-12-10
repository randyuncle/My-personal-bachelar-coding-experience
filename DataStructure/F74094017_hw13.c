#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#define MAX 10000

int parent[MAX];
int print[100000000];

int collapseFind(int i);

void heightUnion(int i, int j)
{
    //if the given node ISN'T the ROOT NODE
    i = collapseFind(i);
    j = collapseFind(j);
    
    //compare the height(note that the height is NEGATIVE)
    if(i != j)
    {
        if(parent[i] > parent[j])
        {
            parent[i] = j;
        }
        else if(parent[i] < parent[j])
        {
            parent[j] = i;
        }
        else
        {
            parent[j] = i;
            parent[i]--;
        }
    }
}

int collapseFind(int i)
{
    int root, trail, lead;

    for(root = i ; parent[root] >= 0 ; root = parent[root]);

    for(trail = i ; trail != root ; trail = lead)
    {
        lead = parent[trail];
        parent[trail] = root;
    }

    return root;
}

bool same(int i, int j)
{
    if(collapseFind(i) == collapseFind(j))
    {
        return true;
    }

    return false; 
}

int main()
{
    int round, n, ops, printCou = 0;

    scanf("%d",&round);
    
    for(int i = 0 ; i < round ; i++)
    {
        scanf("%d %d",&n,&ops);

        for(int j = 0 ; j < n ; j++)
        {
            parent[j] = -1;
        }

        for(int j = 0 ; j < ops ; j++)
        {
            char command[10],*p;
            int a, b;
  
            scanf("%s",command);

            p = command;

            if(strcmp(p,"union") == 0)
            {
                scanf("%d %d",&a,&b);
                heightUnion(a,b);
            }
            else if(strcmp(p,"find") == 0)
            {
                scanf("%d",&a);

                print[printCou++] = collapseFind(a);
            }
            else if(strcmp(p,"same") == 0)
            {
                bool isSame;

                scanf("%d %d",&a,&b);
                isSame = same(a,b);

                if(isSame)
                {
                    print[printCou++] = 10001;
                }
                else
                {
                    print[printCou++] = 10002;
                }
            }
        }
    }

    for(int i = 0 ; i < printCou ; i++)
    {
        if(print[i] == 10001)
        {
            printf("true\n");
        }
        else if(print[i] == 10002)
        {
            printf("false\n");
        }
        else
        {
            printf("%d\n",print[i]);
        } 
    }

    return 0;
}