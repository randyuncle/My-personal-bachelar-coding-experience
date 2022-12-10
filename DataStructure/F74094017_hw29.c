#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100
#define HASHKEY1 3
#define HASHKEY2 7
#define HASHKEY3 5
#define HASHKEY4 1
#define ll long long

int hash[MAX];
bool comfirmArray[MAX];

int hashFunction1(int string){
    ll int hashMethod = string * HASHKEY1;
    return (hashMethod % MAX);
}

int hashFunction2(int string){
    ll int hashMethod = 2 * string * HASHKEY1;
    return (hashMethod % MAX);
}

int hashFunction3(char string[]){
    ll int hashMethod;
    int num = strlen(string);
    for(int i = 0 ; i < num ; i++){
        hashMethod += HASHKEY3 + (int)string[i];
        hashMethod %= MAX;
    }
    return hashMethod;
}

int hashFunction4(char string[]){
    ll int hashMethod;
    int num = strlen(string);
    for(int i = 0 ; i < num ; i++){
        hashMethod += (HASHKEY4 + i) * (int)string[i];
        hashMethod %= MAX;
    }
    return hashMethod;
}

int stringToInt(char *string){
    int num = 0;
    while(*string)
        num += *string++;
    return num;
}

bool lookup(char string[]){
    int stringNum = stringToInt(string), a, b, c, d;
    a = hashFunction1(stringNum);
    b = hashFunction2(stringNum);
    c = hashFunction3(string);
    d = hashFunction4(string);

    if(comfirmArray[a] && comfirmArray[b] && comfirmArray[c] && comfirmArray[d])
        return true;
    
    return false;
}

bool update(char string[]){
    if(lookup(string))//if it's "maybe" the hash
        return false;
    
    int stringNum = stringToInt(string), a, b, c, d;
    a = hashFunction1(stringNum);
    b = hashFunction2(stringNum);
    c = hashFunction3(string);
    d = hashFunction4(string);

    comfirmArray[a] = true;
    comfirmArray[b] = true;
    comfirmArray[c] = true;
    comfirmArray[d] = true;

    return true;
}

void insert(char string[]){
    int stringNum = stringToInt(string), a, b, c, d;
    a = hashFunction1(stringNum);
    b = hashFunction2(stringNum);
    c = hashFunction3(string);
    d = hashFunction4(string);

    comfirmArray[a] = true;
    comfirmArray[b] = true;
    comfirmArray[c] = true;
    comfirmArray[d] = true;
}

int main(){
    double total = 0.0000, collision = 0.0000;
    //initialize
    for(int i = 0 ; i < 35 ; i++){
        char string[MAX];
        scanf("%s", string);
        insert(string);
    }

    while(true){
        char command[MAX], *p;
        scanf("%s", command);
        p = command;
        if(!strcmp(p, "update")){
            total++;
            char string[MAX];
            scanf("%s", string);
            bool canInsert = update(string);
            if(canInsert)
                printf("update success!\n");
            else{
                collision++;
                printf("%s maybe is in the hash table\n",string);
            }
        }
        else
            break;
    }
    double error = collision / total;
    printf("error rate %f", error);

    return 0;
}