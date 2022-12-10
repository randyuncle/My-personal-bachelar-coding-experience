#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct polyNode *polyPointer;

typedef struct polyNode
{
    int coef;
    int expon;
    polyPointer link;
};

char *str[100];
int strCou = 0;

//create the header node which is a null set 
void createPoly(polyPointer *ptr,polyPointer *front)
{
    polyPointer node = (polyPointer)malloc(sizeof(struct polyNode));

    if(node == 0)
    {
        printf("out of memory");
    }
    else
    {
        node->coef = 0;
        node->expon = 0;
        node->link = NULL;

        *ptr = node;
        *front = node;
    }
}

//linked the list
void insertPoly(int coef, int expo, polyPointer *ptr)
{
    polyPointer node = (polyPointer)malloc(sizeof(struct polyNode));
    polyPointer temp;

    temp = *ptr;
    if(ptr != NULL)
    {
        node->coef = coef;
        node->expon = expo;
        node->link = NULL;

        temp->link = node;

        *ptr = temp->link;
    }
    else
    {
        printf("malloc fail to add value");
    }       
}

//have the last node linked to the null header node
void toCircleLink(polyPointer *front, polyPointer *ptr)
{
    polyPointer temp = *ptr;

    temp->link = *front;

    *ptr = temp->link;
}

void addP(polyPointer a, polyPointer b, polyPointer *c)
{
    int sumCoef;
    polyPointer tempR = *c;
    
    a = a->link;
    b = b->link;

    while(a->coef > 0 || b->coef > 0)
    {
        if(a->expon > b->expon)
        {
            insertPoly(a->coef,a->expon,&tempR);
            a = a->link;
        }
        else if(a->expon < b->expon)
        {
            insertPoly(b->coef,b->expon,&tempR);
            b = b->link;
        }
        else
        {
            sumCoef = a->coef + b->coef;

            insertPoly(sumCoef,a->expon,&tempR);

            a = a->link;
            b = b->link;
        }
    }

    *c = tempR;
}

void subP(polyPointer a, polyPointer b, polyPointer *c)
{
    int subCoef;
    polyPointer tempR = *c;

    a = a->link;
    b = b->link;

    while(a->coef > 0 || b->coef > 0)
    {
        if(a->expon > b->expon)
        {
            insertPoly(a->coef,a->expon,&tempR);
            a = a->link;
        }
        else if(a->expon < b->expon)
        {
            insertPoly(-(b->coef),b->expon,&tempR);
            b = b->link;
        }
        else
        {
            subCoef = a->coef - b->coef;

            if(subCoef != 0)
            {
                insertPoly(subCoef,a->expon,&tempR);

            }

            a = a->link;
            b = b->link;
        }
    }
    
    *c = tempR;
}

void multP(polyPointer a, polyPointer b, polyPointer *c)
{
    int mulCoef[100],mulExpon[100],mulCou = 0;
    polyPointer tempR = *c;

    a = a->link;
    b = b->link;

    while(a->coef > 0)
    {
        while(b->coef > 0)
        {
            bool goNext = true;
            mulCoef[mulCou] = a->coef * b->coef;
            mulExpon[mulCou] = a->expon + b->expon;
            
            for(int i = 0 ; i < mulCou ; i++)
            {
                if(mulExpon[i] < mulExpon[mulCou])
                {
                    int temp1 = mulCoef[i];
                    mulCoef[i] = mulCoef[mulCou];
                    mulCoef[mulCou] = temp1;

                    int temp2 = mulExpon[i];
                    mulExpon[i] = mulExpon[mulCou];
                    mulExpon[mulCou] = temp2;
                }
                else if(mulExpon[i] == mulExpon[mulCou])
                {
                    mulCoef[i] += mulCoef[mulCou];
                    goNext = false;

                    break;
                }
            }

            b = b->link;

            if(goNext)
            {
                mulCou++;
            }
        }

        a = a->link;
        b = b->link;
    }

    for(int i = 0 ; i < mulCou ; i++)
    {
        insertPoly(mulCoef[i],mulExpon[i],&tempR);        
    }

    *c = tempR;
}

void erase(polyPointer *ptr)
{
    polyPointer head = *ptr;
    if(*ptr)
    {
        polyPointer temp = (*ptr)->link;
        (*ptr)->link = head;
        head = temp;
        *ptr = NULL;
    }
}

char* write(polyPointer ptr)
{
    int i = 0;
    str[strCou] = (char*)malloc(200*sizeof(char));

    ptr = ptr->link;

    while(ptr->coef != 0)
    {
        char coef[10], expon[10];

        itoa(ptr->coef,coef,10);
        itoa(ptr->expon,expon,10);
        
        if(ptr->coef > 0)
        {
            if(i == 0)
            {
                strcpy(str[strCou] ,coef);
                strcat(str[strCou] ,"x^");
                strcat(str[strCou] ,expon);
                i++;
            }
            else
            {
                strcat(str[strCou] ,"+");
                strcat(str[strCou] ,coef);
                strcat(str[strCou] ,"x^");
                strcat(str[strCou] ,expon);
            }
        }
        else if(ptr->coef < 0)
        {
            if(i == 0)
            {
                strcpy(str[strCou] ,coef);
                strcat(str[strCou] ,"x^");
                strcat(str[strCou] ,expon);
                
                i++;
            }
            else
            {
                strcat(str[strCou] ,coef);
                strcat(str[strCou] ,"x^");
                strcat(str[strCou] ,expon);
            }
        }

        ptr = ptr->link;
    }

    ptr = ptr->link;


    return str[strCou++];
}

float evalP(polyPointer ptr,double eval)
{
    double sum = 0;

    ptr = ptr->link;

    while(ptr->coef != 0)
    {
        if(ptr->coef > 0)
        {
            sum += ptr->coef * pow(eval,ptr->expon);
        }
        else if(ptr->coef < 0)
        {
            sum += ptr->coef * pow(eval,ptr->expon);
        }

        ptr = ptr->link;
    }

    ptr = ptr->link;

    return sum;
}

int main()
{
    polyPointer poly[10];

    polyPointer front = NULL;

    int n,turns,printCou = 0;
    int expTok = 0,polyTok = 0;
    float sumEval = 0.00;
    char command[10] , expSave[50][50] , expIn[50], expIn2[50];
    char* format,*formatR;
    char* print[50];

    scanf("%d",&n);

    for(int i = 0 ; i < n ; i++)
    {
        scanf("%s",command);

        format = command;

        if(strcmp(format,"pread") == 0)
        {
            scanf("%s",expSave[expTok++]);
            scanf("%d",&turns);

            createPoly(&poly[polyTok],&front);

            for(int j = 0 ; j < turns; j++)
            {
                int coef,expon;
                scanf("%d %d",&coef,&expon);

                insertPoly(coef,expon,&poly[polyTok]);
            }

            toCircleLink(&front,&poly[polyTok++]);
            
            print[printCou++] = "ok";
        }
        else if(strcmp(format,"pwrite") == 0)
        {
            scanf("%s",expIn);

            format = expIn;
            int space = 0;

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    space = i;
                    break;
                }
            }

            char*result = write(poly[space]);
            print[printCou++] = result;
        }
        else if(strcmp(format,"perase") == 0)
        {
            scanf("%s",expIn);

            format = expIn;
            int space = 0;

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    space = i;
                    break;
                }
            }

            erase(&poly[space]);

            for(int i = 0 ; i < 50 ; i++)
            {
                if(expSave[space][i] != '\0')
                {
                    expSave[space][i] = '\0';
                }
                else
                {
                    break;
                }
            }
            print[printCou++] = "erase";
        }
        else if(strcmp(format,"padd") == 0)
        {
            scanf("%s",expSave[expTok++]);
            scanf("%s %s",expIn,expIn2);

            format = expIn;
            formatR = expIn2;
            int spaceL = 0 , spaceR = 0;

            createPoly(&poly[polyTok],&front);

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    spaceL = i;
                }
                else if(strcmp(formatR,expSave[i]) == 0)
                {
                    spaceR = i;
                }
            }

            addP(poly[spaceL],poly[spaceR],&poly[polyTok]);
            toCircleLink(&front,&poly[polyTok++]);
            print[printCou++] = "added";
        }
        else if(strcmp(format,"psub") == 0)
        {
            scanf("%s",expSave[expTok++]);
            scanf("%s %s",expIn,expIn2);

            format = expIn;
            formatR = expIn2;
            int spaceL = 0 , spaceR = 0;

            createPoly(&poly[polyTok],&front);

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    spaceL = i;
                }
                else if(strcmp(formatR,expSave[i]) == 0)
                {
                    spaceR = i;
                }
            }

            subP(poly[spaceL],poly[spaceR],&poly[polyTok]);
            toCircleLink(&front,&poly[polyTok++]);
            print[printCou++] = "substracted";
        }
        else if(strcmp(format,"pmult") == 0)
        {
            scanf("%s",expSave[expTok++]);
            scanf("%s %s",expIn,expIn2);

            format = expIn;
            formatR = expIn2;
            int spaceL = 0 , spaceR = 0;

            createPoly(&poly[polyTok],&front);

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    spaceL = i;
                }
                else if(strcmp(formatR,expSave[i]) == 0)
                {
                    spaceR = i;
                }
            }

            multP(poly[spaceL],poly[spaceR],&poly[polyTok]);
            toCircleLink(&front,&poly[polyTok++]);
            print[printCou++] = "multiplied";
        }
        else if(strcmp(format,"eval") == 0)
        {
            scanf("%s",expIn);

            format = expIn;
            int space = 0;

            for(int i = 0 ; i < expTok ; i++)
            {
                if(strcmp(format,expSave[i]) == 0)
                {
                    space = i;
                    break;
                }
            }

            float eval;
            scanf("%f",&eval);

            sumEval = evalP(poly[space],eval);

            char sum[100000];
            sprintf(sum,"%.2f",sumEval);

            print[printCou++] = sum;
        }
    }

    for(int i = 0 ; i < printCou ; i++)
    {
        printf("%s\n",print[i]);
    }

    return 0;
}