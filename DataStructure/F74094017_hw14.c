#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#define MAX 100000

int parentHeight[MAX];
int parentWeight[MAX];

int heightCollapseFind(int i);
int weightCollapseFind(int i);

void heightUnion(int i, int j)
{
    i = heightCollapseFind(i);
    j = heightCollapseFind(j);

    //compare the height(note that the height is NEGATIVE)
    if(i != j)
    {
        if(parentHeight[i] > parentHeight[j])
        {
            parentHeight[i] = j;
        }
        else if(parentHeight[i] < parentHeight[j])
        {
            parentHeight[j] = i;
        }
        else
        {
            parentHeight[j] = i;
            parentHeight[i]--;
        }
    }
}

void weightUnion(int i, int j)
{
    i = weightCollapseFind(i);
    j = weightCollapseFind(j);

    if(i != j)
    {
        //compare the weight(note that the weight is NEGATIVE)
        int temp = parentWeight[i] + parentWeight[j];
        if(parentWeight[i] > parentWeight[j])
        {
            parentWeight[i] = j;
            parentWeight[j] = temp;
        }
        else
        {
            parentWeight[j] = i;
            parentWeight[i] = temp;
        }
    }
}

int heightCollapseFind(int i)
{
    int root, trail, lead;

    for(root = i ; parentHeight[root] >= 0 ; root = parentHeight[root]);

    for(trail = i ; trail != root ; trail = lead)
    {
        lead = parentHeight[trail];
        parentHeight[trail] = root;
    }

    return root;
}

int weightCollapseFind(int i)
{
    int root, trail, lead;

    for(root = i ; parentWeight[root] >= 0 ; root = parentWeight[root]);

    for(trail = i ; trail != root ; trail = lead)
    {
        lead = parentWeight[trail];
        parentWeight[trail] = root;
    }

    return root;
}

int main()
{
    int n, test;
    scanf("%d %d",&n,&test);

    clock_t begin,end;

    //initialization with -1
    for(int j = 0 ; j < n ; j++)
    {
        parentHeight[j] = -1;
        parentWeight[j] = -1;
    }

    int a[MAX], b[MAX];

    //test cases inputs
    for(int i = 0 ; i < test ; i++)
    {
        scanf("%d %d",&a[i],&b[i]);
    }

    //height union efficiency test
    begin = clock();
    for(int i = 0 ; i < test ; i++)
    {
        heightUnion(a[i], b[i]);
    }
    end = clock();

    printf("the time for the height union is %f\n",(double)((end - begin)/CLOCKS_PER_SEC));

    //weight union efficiency test
    begin = clock();
    for(int i = 0 ; i < test ; i++)
    {
        weightUnion(a[i], b[i]);
    }
    end = clock();

    printf("the time for the weight union is %f\n",(double)((end - begin)/CLOCKS_PER_SEC));

    /*
    **(if it needs) the finding root
    the result from weight union and the height union might have some difference since the standard to union is different
    for(int i = 0 ; i < n ; i++)
    {
        int find;
        scanf("%d",&find);

        printf("%d in height union is : %d\n",find,heightCollapseFind(find));
        printf("%d in weight union is : %d\n",find,weightCollapseFind(find));
    }
    */

    return 0;
}