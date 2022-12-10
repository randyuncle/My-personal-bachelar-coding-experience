#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
    struct node* next;
};
struct node* head = NULL;
struct node* tail = NULL;
void MoveToTail();

int main(void) { 
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i) {
        struct node* tmp = malloc(sizeof(struct node));
        scanf("%d", &tmp->val);
        tmp->next = NULL;
        if(i == 1)
            head = tmp;
        else
            tail->next = tmp;
        tail = tmp;
    }
    MoveToTail();
    for(struct node* cur = head; cur != NULL; cur = cur->next)
        printf("%d ", cur->val); 
    return 0;
}void MoveToTail() {
    // "struct node* head" and "struct node* tail" are in global
    // you can use this two pointers to do this problem
    int counter,a = 0;
    struct node* now = head;
    struct node* move = head;
    for( struct node* add = head ; move != NULL ; move = move->next){
        a++;
    }
    for( int i=0 ; move != NULL ; move = move->next,i++){
        counter = 0;
        for( ; now != NULL ; now = now->next){
            if(move->val > now->val){
                counter++;
            }
        }
        if(counter == (a-1)){
            tail = move;
            head = move;
            break;
            
        }
    }
}