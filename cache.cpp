#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <cmath>

#define DM 0
#define FW 1
#define FA 2
#define FIFO 0
#define LRU 1
#define lli long long int

using namespace std;

/**
 * @brief the least used is on the leftist
 * 
 * @param address 
 * @param index 
 * @param set 
 * @param re 
 */
void LRUupdate(lli *cache, int place, int index, int replace){
    if(replace == FIFO) return;
    if(index == 1) return;

    int temp = cache[place], i;
    for(i = place ; i < index - 1 ; i++){
        if(cache[i + 1] == 0) break;
        cache[i] = cache[i + 1];
    }
    cache[i] = temp;
}

//int main(int argc, char *argv[])
int main(int argc, char *argv[]){
    fstream file_in, file_out;
    file_in.open(argv[1], ios::in);
    file_out.open(argv[2], ios::out);

    int cache_size , block_size, associativity, replace;
    file_in >> cache_size >> block_size >> associativity >> replace;
    
    double turns = 0.000000, miss = 0.000000;
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
            index = 4;
            set = cache_size / (block_size * 4);
            break;
        default: //FA
            index = (cache_size / block_size);
            set = 1;
            break;
    }

    /**
     * @brief hit check
     * cache -> a 2D array
     * time -> FIFO replace save
     */
    lli cache[set][index], time[set], address;
    bool inCache = false;
    memset(cache, 0, sizeof(cache));//cache initialized to zero
    memset(time, 0, sizeof(time));

    while(file_in >> address){
        turns++;
        lli block_address = floor(address / block_size);
        int s = block_address % set; //get which set the address goes
        int tag = floor(block_address / set);

        for(int i = 0 ; i < index ; i++){
            if(cache[s][i] == 0){ //there's nothing
                cache[s][i] = tag;
                file_out << -1 << endl;

                LRUupdate(cache[s], i, index, replace);
                inCache = true;
                miss++;
                break;
            }
            else if(cache[s][i] == tag){ //hit!
                file_out << -1 << endl;

                LRUupdate(cache[s], i, index, replace);
                inCache = true;
                break;
            }
        }

        /**
         * @brief doesn't hit anything and the space is full -> do the replacement
         * 
         */
        if(!inCache){
            int re;
            switch(replace){
                case FIFO:
                    re = time[s];
                    file_out << cache[s][re] << endl;

                    cache[s][re] = tag;
                    time[s] = (re + 1) % index;
                    break;
                case LRU:
                    re = 0;
                    file_out << cache[s][re] << endl;

                    cache[s][re] = tag;
                    LRUupdate(cache[s], re, index, replace); //reset the frequency
                    break;
            }
            miss++;
        }
        inCache = false;
    }

    file_out << "Miss rate = "  << fixed << setprecision(6)  << miss / turns;
    return 0;
}
