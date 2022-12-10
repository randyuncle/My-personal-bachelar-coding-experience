#include <iostream>
#define MAX 1000005

using namespace std;

typedef struct queue{
    int ch;
    int parag;
    queue* next;
    queue* parent;
}queue;
queue *rear, *front;
queue *p[MAX] = {NULL};
int size = 0;

void sizeOut(){
    front->parent->next = NULL;
    int ch = front->ch;
    queue* temp = front;
    front = front->parent;
    p[ch] = NULL;
    free(temp);
}

void nodeInsert(int ch, int parag){
    rear->ch = ch;
    rear->parag = parag;
    p[ch] = rear;

    rear->parent = (queue*)malloc(sizeof(queue));
    rear->parent->next = rear;
    rear = rear->parent;
}

void nodeRefresh(int ch, int parag){
    if(p[ch] == rear->next){
        p[ch]->parag = parag;
        return;
    }
    else if(p[ch] == front){
        sizeOut();
        nodeInsert(ch,parag);
        return;
    }
    p[ch]->parent->next = p[ch]->next;
    p[ch]->next->parent = p[ch]->parent;

    queue* temp = p[ch];
    p[ch] = NULL;
    free(temp);
    nodeInsert(ch,parag);
    return;
}

void set(int n){
    int ch, parag;
    cin >> ch >> parag;

    if(p[ch]) nodeRefresh(ch, parag);
    else nodeInsert(ch, parag), size++;

    if(size > n) sizeOut(), size--;
}

void get(){
    int ch;
    cin >> ch;

    if(p[ch]){
        cout << p[ch]->parag << endl;
        nodeRefresh(ch, p[ch]->parag);
    }
    else cout << -1 << endl;
}

int main(){
    int n, m, input;
    cin >> n >> m;

    rear = (queue*)malloc(sizeof(queue));
    front = rear;
    for(int i = 0 ; i < m ; i++){
        string command; 
        cin >> command;
        if(command == "set") set(n);
        else if(command == "get") get();
    }

    return 0;
}