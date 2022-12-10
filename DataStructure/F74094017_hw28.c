#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 17

int hashTable[MAX], nodes = 0;
bool overflow = false;

void insert(int key){
    if(nodes == MAX){
        overflow = true;
        return;
    }else{
        overflow = false;
    }

    int place = key % MAX;
    if(hashTable[place] < 0){
        hashTable[place] = key;
        return;
    }

    while(hashTable[place] > -1)
        place = (place + 1) % MAX;

    hashTable[place] = key;
    nodes++;
    return;
}

int search(int key){
    int home, curr;
    home = key % MAX;
    for(curr = home ; (hashTable[curr] > -1) && (hashTable[curr] != key) ;){
        curr = (curr + 1) % MAX;
        if(home == curr)
            return -1;
    }
    if(hashTable[curr] == key)
        return curr;
    return -1;
}

/**
 * @brief left shift the nodes next to it to the changed bucket till there's no fordable node
 * 
 * @param bucket 
 */
void linearMinShift(int bucket){
    int curr = bucket;
    while(true){
        curr = (curr + 1) % MAX;

        if((hashTable[curr] > -1)){
            if((hashTable[curr] % MAX != curr)){
                if(bucket < curr){
                    hashTable[bucket] = hashTable[curr];
                    hashTable[curr] = -1;
                    bucket = curr;
                }else if((bucket > (hashTable[curr] % MAX) && (((hashTable[curr] % MAX) - curr) > 0))){
                    hashTable[bucket] = hashTable[curr];
                    hashTable[curr] = -1;
                    bucket = curr;
                }
            }else
                continue;
        }else
            break;
    }
    return;
}

void delete(int key){
    int bucket = search(key);
    if(bucket == -1)
        return;
    hashTable[bucket] = -1;
    nodes--;
    linearMinShift(bucket);
    if(overflow)
        overflow = false;
    return;
}

void printHash(char command[]){
    printf("%s | ", command);
    for(int i = 0 ; i < MAX ; i++)
        printf("%d ", hashTable[i]);
    printf("\n");
}

int main(){
    //Initialize
    for(int i = 0 ; i < MAX ; i++)
        hashTable[i] = -1;
    
    while(true){
        int key;
        char command[10], *p;
        scanf("%s", command);
        p = command;

        if(!strcmp(p, "insert")){
            scanf("%d", &key);
            insert(key);
            if(overflow){
                printf("overflow\n");
            }
        }else if(!strcmp(p, "search")){
            scanf("%d", &key);
            int find = search(key);
            if(find > -1) printf("find %d in node #%d\n", key, find);
            else printf("the node isn't in the hash table\n");
        }else if(!strcmp(p, "delete")){
            scanf("%d", &key);
            delete(key);
        }else if(!strcmp(p, "quit")){
            break;
        }else{
            printf("Invalid command!\n");
        }

        printHash(command);
    }

    return 0;
}