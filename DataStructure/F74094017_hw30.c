#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief patricia tries structure
 * 
 */
typedef struct tries * patriciaPointer;
struct tries
{
    patriciaPointer leftchild;
    char key[1000];
    int bitNum;
    int val;
    patriciaPointer rightchild;
};
patriciaPointer ptree;
int operations;

int delbitval;

/**
 * @brief print the results
 * 
 */
struct print{
    int method;
    int value;
    struct print * next;
};
struct print* printlist;
struct print* rear;

/**
 * @brief Create a Node object
 * 
 * @param bitNum 
 * @param val 
 * @return patriciaPointer 
 */
patriciaPointer createNode(int bitNum,int val){
    patriciaPointer node = (patriciaPointer)malloc(sizeof(struct tries));

    node->leftchild = node;
    node->bitNum = bitNum;
    node->val = val;
    node->rightchild = node;

    return node;
}

/**
 * @brief search in a patricia tries
 * 
 * @param tree 
 * @param key 
 * @return patriciaPointer 
 */
patriciaPointer search(patriciaPointer tree, char key[]){
    patriciaPointer curr, next;

    if(!tree){
        return NULL;
    }

    next = tree->leftchild;
    curr = tree;

    while(next->bitNum > curr->bitNum){
        curr = next;
        
        if(key[next->bitNum - 1] == '1')
            next = next->rightchild;
        else
            next = next->leftchild;
    }

    return next;
}

/**
 * @brief insert nodes to patricia tries
 * 
 * @param tree 
 * @param node 
 * @return true 
 * @return false 
 */
bool insertNode(patriciaPointer *tree, patriciaPointer *node){
    if(!(*tree)){
        (*tree) = (*node);
        return true;
    }

    patriciaPointer curr, parent;
    patriciaPointer prev = search((*tree), (*node)->key);
    int i;

    if(strcmp(prev->key, (*node)->key) == 0)
        return false;

    for(i = 1 ; (prev->key[i - 1] == (*node)->key[i - 1]) ; i++);

    curr = (*tree)->leftchild;
    parent = (*tree);
    while((curr->bitNum > parent->bitNum) && (curr->bitNum < i)){
        parent = curr;

        if((*node)->key[curr->bitNum - 1] == '1')
            curr = curr->rightchild;
        else
            curr = curr->leftchild;
    }

    (*node)->bitNum = i;

    if((*node)->key[i - 1] == '1')
        (*node)->leftchild = curr;
    else
        (*node)->rightchild = curr;

    if(curr == parent->leftchild)
        parent->leftchild = (*node);  
    else
        parent->rightchild = (*node); 

    return true;
}

int deleteNode(patriciaPointer *tree, char key[]){
    if(!(*tree)){
        return -1;
    }

    patriciaPointer curr = (*tree)->leftchild;
    patriciaPointer parent = (*tree);
    patriciaPointer grandparent;
    while(curr->bitNum > parent->bitNum){
        grandparent = parent;
        parent = curr;

        if(key[curr->bitNum - 1] == '1')
            curr = curr->rightchild;
        else
            curr = curr->leftchild;
    }

    if(strcmp(key,curr->key) != 0){
        return -1;
    }

    if(curr == ptree){
        delbitval = curr->val;
        ptree = NULL;
        return delbitval;
    }

    delbitval = curr->val;

    strcpy(curr->key, parent->key);
    curr->val = parent->val;
    if(grandparent->leftchild == parent)
        grandparent->leftchild = parent->leftchild;
    else
        grandparent->rightchild = parent->rightchild;
    parent = NULL;

    return delbitval;
}

int main(){
    scanf("%d",&operations);
    printlist = (struct print*)malloc(sizeof(struct print));

    while(true){
        if(!rear)
            rear = printlist;

        char command[10], *p;
        scanf("%s",command);
        p = command;

        if(strcmp(p,"insert") == 0){
            printlist->next = (struct print*)malloc(sizeof(struct print));
            printlist = printlist->next;

            char key[operations];
            int val;
            scanf("%s %d",key,&val);

            patriciaPointer newNode = createNode(0, val);
            strcpy(newNode->key, key);

            bool repeat = insertNode(&ptree, &newNode);
            
            printlist->method = 0;
            if(repeat)
                printlist->value = val;
            else
                printlist->value = -1;
        }else if(strcmp(p,"search") == 0){
            printlist->next = (struct print*)malloc(sizeof(struct print));
            printlist = printlist->next;

            char key[operations];
            scanf("%s",key);

            patriciaPointer found = search(ptree, key);

            printlist->method = 1;
            if(!found)
                printlist->value = -1;
            else if(!strcmp(found->key, key))
                printlist->value = found->val;
            else
                printlist->value = -1;
        }else if(strcmp(p,"delete") == 0){
            printlist->next = (struct print*)malloc(sizeof(struct print));
            printlist = printlist->next;

            char key[operations];
            scanf("%s",key);

            int isDelete = deleteNode(&ptree, key);

            printlist->method = 2;
            if(isDelete > -1){
                printlist->value = isDelete;
            }
            else
                printlist->value = -1;
        }else if(strcmp(p,"quit") == 0){
            printlist->next = NULL;
            break;
        }
    }

    for(printlist = rear->next ; printlist ; printlist = printlist->next){
        if(printlist->method == 0){
            if(printlist->value < 0)
                printf("intsert -> conflict\n");
            else
                printf("intsert -> %d\n", printlist->value);
        }else if(printlist->method == 1){
            if(printlist->value < 0)
                printf("search -> not found\n");
            else
                printf("search -> %d\n", printlist->value);
        }else if(printlist->method == 2){
            if(printlist->value < 0)
                printf("delete -> not found\n");
            else
                printf("delete -> %d\n", printlist->value);
        }
    }

    return 0;
}