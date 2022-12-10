#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAX 500000
#define BLACK 0
#define RED 1

/*
struct cases{
    int motion;
    int num;
};
*/

/**
 * @brief AVL tree
 * 
 */
typedef struct avlNode* avlpointer;
struct avlNode
{
    avlpointer leftchild;
    int data;
    int height;
    avlpointer rightchild;
};
avlpointer avltree;

/**
 * @brief red black tree
 * 
 */
typedef struct rbNode* redblackpointer;
struct rbNode
{
    redblackpointer leftchild;
    redblackpointer rightchild;
    redblackpointer parent;
    int color; //0 : black, 1 : red
    int data;
};
redblackpointer rbtree;


//second field - 0 : 0->search, 1->exist
//second field - 1 : 0->not exist, 1->exist
//second field - 2 : save the bf or color if it exists(for search)
int print[MAX + 1][3], count = 0;

int height(avlpointer node) 
{
    if(!node)
    {
        return 0;
    }
        
    return node->height;
}

int max(int a, int b) 
{
    if(a > b)
    {
        return a;
    }

    return b;
}

avlpointer createNode(int data)
{
    avlpointer node = (avlpointer)malloc(sizeof(struct avlNode));

    node->leftchild = NULL;
    node->data = data;
    node->height = 1;
    node->rightchild = NULL;

    return node;
}

avlpointer lRotate(avlpointer parent)
{
    avlpointer right = parent->rightchild;
    avlpointer left = right->leftchild;

    right->leftchild = parent;
    parent->rightchild = left;

    parent->height = max(height(parent->leftchild), height(parent->rightchild)) + 1;
    right->height = max(height(right->leftchild), height(right->rightchild)) + 1;

    return right;
}

avlpointer rRotate(avlpointer parent)
{
    avlpointer left = parent->leftchild;
    avlpointer right = left->rightchild;

    left->rightchild = parent;
    parent->leftchild = right;

    parent->height = max(height(parent->leftchild), height(parent->rightchild)) + 1;
    left->height = max(height(left->leftchild), height(left->rightchild)) + 1;

    return left;
}

int balanceFactor(avlpointer node)
{
    if (!node)
    {
        return 0;
    }

    return (height(node->leftchild) - height(node->rightchild));
}

avlpointer avlInsert(avlpointer parent, int data)
{
    if(!parent)
    {
        return createNode(data);
    }
    else if(data < parent->data)
    {
        parent->leftchild = avlInsert(parent->leftchild, data); 
    }
    else if(data > parent->data)
    {
        parent->rightchild = avlInsert(parent->rightchild, data);
    }

    parent->height = 1 + max(height(parent->leftchild), height(parent->rightchild));
    int balance = balanceFactor(parent);

    if(balance > 1 && data < parent->leftchild->data)
    {
        return rRotate(parent);
    }

    if(balance < -1 && data > parent->rightchild->data)
    {
        return lRotate(parent);
    }

    if(balance > 1 && data > parent->leftchild->data)
    {
        parent->leftchild = lRotate(parent->leftchild);
        return rRotate(parent);
    }

    if(balance < -1 && data < parent->rightchild->data)
    {
        parent->rightchild = rRotate(parent->rightchild);
        return lRotate(parent);
    }

    return parent;
}

avlpointer avltreeSearch(avlpointer tree,int data)
{
    while(tree && (data != tree->data))
    {
        if(data < tree->data)
        {
            tree = tree->leftchild;
        }
        else
        {
            tree = tree->rightchild;
        }
    }

    return tree;
}

avlpointer avlFoundMin(avlpointer tree)
{
    avlpointer curr = tree;

    while(curr->leftchild)
    {
        curr = curr->leftchild;
    }

    return curr;
}

avlpointer avlDelete(avlpointer tree, int data)
{
    if(!tree)
    {
        return tree;
    }

    if(data < tree->data)
    {
        tree->leftchild = avlDelete(tree->leftchild, data);
    }
    else if(data > tree->data)
    {
        tree->rightchild = avlDelete(tree->rightchild, data);
    }
    else
    {
        if((!tree->leftchild) || (!tree->rightchild))
        {
            avlpointer temp;
            if(tree->leftchild)
            {
                temp = tree->leftchild;
                *tree = *temp;
            }
            else if(tree->rightchild)
            {
                temp = tree->rightchild;
                *tree = *temp;
            }
            else
            {
                temp = tree;
                tree = NULL;
            }

            temp = NULL;
        }
        else
        {
            avlpointer temp = avlFoundMin(tree->rightchild);

            tree->data = temp->data;
            tree->rightchild = avlDelete(tree->rightchild, temp->data);
        }
    }

    if(!tree)
    {
        return tree;
    }

    tree->height = 1 + max(height(tree->leftchild), height(tree->rightchild));
    int balance = balanceFactor(tree);

    if(balance > 1 && balanceFactor(tree->leftchild) >= 0)
    {
        return rRotate(tree);
    }

    if(balance > 1 && balanceFactor(tree->leftchild) < 0)
    {
        tree->leftchild = lRotate(tree->leftchild);
        return rRotate(tree);
    }

    if(balance < -1 && balanceFactor(tree->rightchild) <= 0)
    {
        return lRotate(tree);
    }

    if(balance < -1 && balanceFactor(tree->rightchild) > 0)
    {
        tree->rightchild = rRotate(tree->rightchild);
        return lRotate(tree);
    }

    return tree;
}

/************************************************************************************/

redblackpointer rbcreateNode(int data)
{
    redblackpointer node = (redblackpointer)malloc(sizeof(struct rbNode));

    node->color = RED;
    node->data = data;
    node->leftchild = NULL;
    node->parent = NULL;
    node->rightchild = NULL;

    return node;
}

void leftRotate(redblackpointer *curr)
{
    redblackpointer right = (*curr)->rightchild;

    (*curr)->rightchild = right->leftchild;

    if((*curr)->rightchild)
    {
        (*curr)->rightchild->parent = (*curr);
    }

    right->parent = (*curr)->parent;

    if(!(*curr)->parent)
    {
        rbtree = right;
    }
    else if((*curr) == (*curr)->parent->leftchild)
    {
        (*curr)->parent->leftchild = right;
    }
    else
    {
        (*curr)->parent->rightchild = right;
    }

    right->leftchild = (*curr);
    (*curr)->parent = right;

    return;
}

void rightRotate(redblackpointer *curr)
{
    redblackpointer left = (*curr)->leftchild;

    (*curr)->leftchild = left->rightchild;

    if((*curr)->leftchild)
    {
        (*curr)->leftchild->parent = (*curr);
    }

    left->parent = (*curr)->parent;

    if(!(*curr)->parent)
    {
        rbtree = left;
    }
    else if((*curr) == (*curr)->parent->leftchild)
    {
        (*curr)->parent->leftchild = left;
    }
    else
    {
        (*curr)->parent->rightchild = left;
    }

    left->rightchild = (*curr);
    (*curr)->parent = left;

    return;
}

void rbInsertFixUp(redblackpointer *curr)
{
    while(((*curr) != rbtree) && ((*curr)->color != BLACK) && ((*curr)->parent->color == RED)) //chk if parent is red
    {
        redblackpointer grandparent = (*curr)->parent->parent;
        redblackpointer parent = (*curr)->parent;

        if(parent == grandparent->leftchild) //parent is the left of the grandparent
        {
            redblackpointer uncle = grandparent->rightchild;

            if(uncle && uncle->color == RED) //case 1 : uncle is red
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                (*curr) = grandparent;
            }
            else 
            {
                //case 2 : uncle is black or NULL and current is the right of the parent
                if((*curr) == parent->rightchild)
                {
                    leftRotate(&parent);
                    (*curr) = parent;
                    parent = (*curr)->parent;
                }

                //case 3 : uncle is black or NULL and if the grandparent needs to rotate
                rightRotate(&grandparent);
                //color chg
                int c = parent->color;
                parent->color = grandparent->color;
                grandparent->color = c;
                (*curr) = parent;
            }
        }
        else //parent is the right of the grandparent
        {
            redblackpointer uncle = grandparent->leftchild;

            if(uncle && uncle->color == RED) //case 1 : uncle is red
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                (*curr) = grandparent;
            }
            else
            {
                //case 2 : uncle is black or NULL and current is the right of the parent
                if((*curr) == parent->leftchild)
                {
                    rightRotate(&parent);
                    (*curr) = parent;
                    parent = (*curr)->parent;
                }

                //case 3 : uncle is black or NULL and if the grandparent needs to rotate
                leftRotate(&grandparent);
                //color chg
                int c = parent->color;
                parent->color = grandparent->color;
                grandparent->color = c;

                (*curr) = parent;
            }
        }
    }

    rbtree->color = BLACK;

    return;
}

void rbInsert(int data)
{
    redblackpointer curr = rbtree;
    redblackpointer prev = NULL;
    redblackpointer new = rbcreateNode(data);

    while (curr)
    {
        prev = curr;

        if(new->data < curr->data)
        {
            curr = curr->leftchild;
        }
        else if(new->data == curr->data) //if repeated, get rid of it
        {
            return;
        }
        else 
        {
            curr = curr->rightchild;
        }
    }

    new->parent = prev;
    
    if(!prev)
    {
        rbtree = new;
    }
    else if(new->data < prev->data)
    {
        prev->leftchild = new;
    }
    else
    {
        prev->rightchild = new;
    }

    rbInsertFixUp(&new);

    return;
}

void rbDeleteFixUp(redblackpointer node)
{
    while(node && (node != rbtree) && node->color == BLACK)
    {
        redblackpointer parent = node->parent;

        if(node == parent->leftchild)
        {
            redblackpointer sibling = NULL;

            if(sibling && (sibling->color == RED)) //case 2
            {
                sibling->color = BLACK;
                parent->color = RED;
                leftRotate(&parent);
                sibling = parent->rightchild;
            }

            if(sibling)
            {
                int slcolor;
                int rlcolor;

                if(sibling->leftchild)
                {
                    slcolor = sibling->leftchild->color;
                }
                else
                {
                    slcolor = BLACK;
                }

                if(sibling->rightchild)
                {
                    rlcolor = sibling->rightchild->color;
                }
                else
                {
                    rlcolor = BLACK;
                }

                if(!slcolor && !rlcolor)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if(rlcolor == BLACK)
                    {
                        sibling->leftchild->color = BLACK;
                        sibling->color = RED;
                        rightRotate(&sibling);
                        sibling = parent->rightchild;
                    }

                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->rightchild->color = BLACK;
                    leftRotate(&parent);
                    node = rbtree;
                }
            }
            else
            {
                node = rbtree;
            }
        }
        else
        {
            redblackpointer sibling = parent->leftchild;

            if(sibling && sibling->color == RED) //case 2
            {
                sibling->color = BLACK;
                parent->color = RED;
                leftRotate(&parent);
                sibling = parent->leftchild;
            }

            if(sibling)
            {
                int slcolor;
                int rlcolor;
                
                if(sibling->leftchild)
                {
                    slcolor = sibling->leftchild->color;
                }
                else
                {
                    slcolor = BLACK;
                }

                if(sibling->rightchild)
                {
                    rlcolor = sibling->rightchild->color;
                }
                else
                {
                    rlcolor = BLACK;
                }

                if(!slcolor && !rlcolor)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if(slcolor == BLACK)
                    {
                        sibling->rightchild->color = BLACK;
                        sibling->color = RED;
                        leftRotate(&sibling);
                        sibling = parent->leftchild;
                    }

                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->leftchild->color = BLACK;
                    rightRotate(&parent);
                    node = rbtree;
                }
            }
            else
            {
                node = rbtree;
            }
        }
    }

    node->color = BLACK;
    return;
}

redblackpointer findMin(redblackpointer curr)
{
    if(!curr->leftchild)
    {
        return curr;
    }
    else
    {
        return findMin(curr->leftchild);
    }
}

redblackpointer successor(redblackpointer curr)
{
    if(curr->rightchild)
    {
        return findMin(curr->rightchild);
    }

    redblackpointer new = curr->parent;

    while(new && (curr == new->rightchild))
    {
        curr = new;
        new = new->parent;
    }

    return new;
}

redblackpointer rbtreeSearch(redblackpointer tree, int data)
{
    while (tree)
    {
        if(data == tree->data)
        {
            break;
        }
        if(data < tree->data)
        {
            tree = tree->leftchild;
        }
        else
        {
            tree = tree->rightchild;
        }
    }
    
    return tree;
}

void rbDelete(int data)
{
    redblackpointer del = rbtreeSearch(rbtree, data);

    if(!del)
    {
        return;
    }

    redblackpointer child;
    redblackpointer temp;

    if(!del->leftchild || !del->rightchild)
    {
        temp = del;
    }
    else
    {
        temp = successor(del);
    }

    if(temp->leftchild)
    {
        child = temp->leftchild;
    }
    else
    {
        child = temp->rightchild;
    }

    if(child)
    {
        child->parent = temp->parent;
    }
    
    if(!temp->parent)
    {
        rbtree = child;
    }
    else if(temp == temp->parent->leftchild)
    {
        temp->parent->leftchild = child;
    }
    else
    {
        temp->parent->rightchild = child;
    }

    if(temp != del)
    {
        del->data = temp->data;
        del->color = temp->color;
    }

    if(child && temp->color == BLACK)
    {
        rbDeleteFixUp(child);
    }

    return;
}


int main()
{
    char command[10], *p;
    scanf("%s",command);
    p = command;

    if(strcmp(p,"AVL") == 0)
    {
        avlpointer answer;

        while(true)
        {
            scanf("%s",command);
            p = command;

            if(strcmp(p,"insert") == 0)
            {
                int data;
                scanf("%d",&data);
                avltree = avlInsert(avltree, data);
            }
            else if(strcmp(p,"search") == 0)
            {
                int data;
                scanf("%d",&data);
                answer = avltreeSearch(avltree, data);

                print[count][0] = 0;
                if(answer)
                {
                    print[count][1] = 1;
                    print[count++][2] = balanceFactor(answer);
                }
                else
                {
                    print[count][1] = 0;
                    print[count++][2] = -1;
                }
                answer = NULL;
            }
            else if(strcmp(p,"delete") == 0)
            {
                int data;
                scanf("%d",&data);
                avltree = avlDelete(avltree, data);
            }
            else if(strcmp(p,"exist") == 0)
            {
                int data;
                scanf("%d",&data);
                answer = avltreeSearch(avltree, data);

                print[count][0] = 1;
                if(answer)
                {
                    print[count][1] = 1;
                    print[count++][2] = 0;
                }
                else
                {
                    print[count][1] = 0;
                    print[count++][2] = -1;
                }
                answer = NULL;
            }
            else if(strcmp(p,"quit") == 0)
            {
                for(int i = 0 ; i < count ; i++)
                {
                    if(!print[i][0]) //search
                    {
                        if(print[i][1])
                        {
                            printf("%d\n",print[i][2]);
                        }
                        else
                        {
                            printf("Not found\n");
                        }
                    }
                    else //exist
                    {
                        if(print[i][1])
                        {
                            printf("exist\n");
                        }
                        else
                        {
                            printf("Not exist\n");
                        }
                    }
                }
                break;
            }
        }
    }
    else if(strcmp(p,"red_black") == 0)
    {
        redblackpointer answer;

        while(true)
        {
            scanf("%s",command);
            p = command;

            if(strcmp(p,"insert") == 0)
            {
                int data;
                scanf("%d",&data);
                rbInsert(data);
            }
            else if(strcmp(p,"search") == 0)
            {
                int data;
                scanf("%d",&data);
                answer = rbtreeSearch(rbtree, data);

                print[count][0] = 0;
                if(answer)
                {
                    print[count][1] = 1;
                    print[count++][2] = answer->color;
                }
                else
                {
                    print[count][1] = 0;
                    print[count++][2] = -1;
                }
            }
            else if(strcmp(p,"delete") == 0)
            {
                int data;
                scanf("%d",&data);
                rbDelete(data);
            }
            else if(strcmp(p,"exist") == 0)
            {
                int data;
                scanf("%d",&data);
                answer = rbtreeSearch(rbtree, data);

                print[count][0] = 1;
                if(answer)
                {
                    print[count][1] = 1;
                    print[count++][2] = 0;
                }
                else
                {
                    print[count][1] = 0;
                    print[count++][2] = -1;
                }
            }
            else if(strcmp(p,"quit") == 0)
            {
                for(int i = 0 ; i < count ; i++)
                {
                    if(!print[i][0]) //search
                    {
                        if(print[i][1])
                        {
                            if(print[i][2] == BLACK)
                            {
                                printf("black\n");
                            }
                            else
                            {
                                printf("red\n");
                            }
                        }
                        else
                        {
                            printf("Not found\n");
                        }
                    }
                    else //exist
                    {
                        if(print[i][1])
                        {
                            printf("exist\n");
                        }
                        else
                        {
                            printf("Not exist\n");
                        }
                    }
                }
                break;
            }
        }
        
    }
    

    return 0;
}

/*
int main()
{
    int max = 100000;
    //struct cases m[max];
    //struct cases test[4*max];
    int m[100000];
    int motion[100000];
    int num[100000];
    clock_t begin, end;
    srand((unsigned)time(NULL));

    
    for(int i = 0 ; i < max ; i++)
    {
        int data = rand() % 100000;
        m[i] = data;
    }

    for(int j = 1 ; j <= 4 ; j *= 2)
    {
        avltree = NULL;
        rbtree = NULL;

        double avltime = 0.000000;
        double rbtime = 0.000000;
        unsigned long long int ins = 0, del = 0, seac = 0;

        //pading the initial data
        for(long int i = 0 ; i < max ; i++)
        {
            avltree = avlInsert(avltree, m[i]);
            rbInsert(m[i]);
        }

        for(int t = 0 ; t < j ; t++)
        {
            //test cases making
            for(long int i = 0 ; i < max ; i++)
            {
                int mo = rand() % 4;
                int data = rand() % 100000;

                switch(mo)
                {
                    case 0 : //insert
                        ins++;
                        motion[i] = 0;
                        num[i] = data;
                        break;
                    case 1 : //delete
                        del++;
                        motion[i] = 1;
                        num[i] = data;
                        break;
                    default : //search
                        seac++;
                        motion[i] = 2;
                        num[i] = data;
                        break;
                }
            }

            begin = clock();
            for(int i = 0 ; i < max ; i++)
            {
                avlpointer avls;
                
                switch(motion[i])
                {
                    case 0 : //insert
                        avltree = avlInsert(avltree, num[i]);
                        break;
                    case 1 : //delete
                        avltree = avlDelete(avltree, num[i]);
                        break;
                    default : //search
                        avls = avltreeSearch(avltree, num[i]);
                        break;
                }
            }
            end = clock();
            avltime += (double)(end-begin);

            begin = clock();
            for(long int i = 0 ; i < max ; i++)
            {
                redblackpointer rbs;

                switch(motion[i])
                {
                    case 0 : //insert
                        rbInsert(num[i]);
                        break;
                    case 1 : //delete
                        rbDelete(num[i]);
                        break;
                    default : //search
                        rbs = rbtreeSearch(rbtree, num[i]);
                        break;
                }
            }
            end = clock();
            rbtime += (double)(end-begin);
        }
        printf("insert : %llu, delete : %llu, search : %llu\n", ins, del, seac);
        printf("avltree : n = %d and m = %dn test cases costs : %f \n",max,j*max,avltime);
        printf("rbtree : n = %d and m = %dn test cases costs : %f \n",max,j*max,rbtime);
    }

    return 0;
}
*/
