#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <cmath>

#define DM 0
#define FW 1
#define FA 2
#define FIFO 0
#define LRU 1
#define lli long long int

using namespace std;

typedef struct node *list;
typedef struct node{
    list next, parent;
    int index; //which block in the set the value is in
}node;
list front[512], rear[512]; //dequeue LRU
unordered_map<lli, list> h; //save the pointer (use the original word to mark)

void newNode(int index, lli address,int set){
    list n = (list)malloc(sizeof(node));
    n->next = NULL;
    n->index = index;

    if(!front[set]){
        n->parent = NULL;
        front[set] = n;
        rear[set] = n;
    }else{
        n->parent = front[set];
        front[set]->next = n;
        front[set] = front[set]->next;
    }

    h.insert(pair<int, list>(address, n));
}

void frequencyChk(unsigned int address, int index, int set, int re){
    if(re == FIFO) return;

    if(!h.count(address))
        newNode(index, address, set);

    list curr = h[address];
    if(curr == front[set]) return;
    else{
        curr->parent->next = curr->next;
        curr->next->parent = curr->parent;

        curr->parent = front[set];
        curr->next = NULL;
        front[set] = front[set]->next;
    }
}

int main(int argc, char *argv[]){
    fstream file_in, file_out;
    file_in.open(argv[1], ios::in);
    file_out.open(argv[2], ios::out);

    int cache_size , block_size, associativity, replace;
    file_in >> cache_size >> block_size >> associativity >> replace;
    
    int turns = 0, miss = 0;
    int set, index;
    /**
     * @brief define the associativity 
     * 
     */
    switch(associativity){
        case DM:
            index = 1;
            set = cache_size / block_size;
            break;
        case FW:
            index = (cache_size / block_size) / 4;
            set = 4;
            break;
        default: //FA
            index = (cache_size / block_size);
            set = 1;
            break;
    }

    /**
     * @brief hit check
     * cache -> save the value!!
     * time -> FIFO replace save
     */
    int cache[set][index], time[set]; 
    lli address;
    bool inCache = false;
    memset(cache, -1, sizeof(cache));
    memset(time, 0, sizeof(time));
    while(file_in >> address){
        turns++;
        int tag = address % set;
        int value = floor(address / set);
        for(int i = 0 ; i < index ; i++){
            if(cache[tag][i] == -1){ //there's nothing
                cache[tag][i] = value;
                printf("-1\n");

                frequencyChk(address, i, tag, replace);
                inCache = true;
                miss++;
                break;
            }
            else if(cache[tag][i] == address){ //hit!
                printf("%d\n", cache[tag][i]);
                frequencyChk(address, i, tag, replace);
                inCache = true;
                break;
            }
        }

        /**
         * @brief doesn't hit anything or the space is full -> do the replacement
         * 
         */
        if(!inCache){
            int re;
            list least = rear[tag];
            switch(replace){
                case FIFO:
                    re = time[tag];
                    printf("%d\n", cache[tag][re]);

                    cache[tag][re] = value;
                    time[tag] = (re + 1) % index;
                    break;
                case LRU:
                    re = least->index;
                    int add = cache[tag][re] * tag + re;
                    h.erase(add);
                    printf("%d\n", cache[tag][re]);

                    cache[tag][re] = value;
                    rear[tag] = rear[tag]->next;         
                    frequencyChk(least->index, address, tag, re); //reset the frequency
                    break;
            }
            miss++;
        }
        inCache = false;
    }

    cout << miss / turns;
    return 0;
}