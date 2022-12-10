#include <iostream>
#include <cstdlib>
#define MAX 15
#define LOTTARY 6

using namespace std;

int roundVisit[MAX]; //mind the place to print when the condition meets to LOTTARY

/**
 * @brief it turns out to be a DFS-like finding(
 * 
 * @param sort 
 * @param k 
 * @param now 
 * @param start 
 */
void printComb(int sort[], int k, int now, int start){
    if(now == LOTTARY){
        for(int i = 0 ; i < LOTTARY ; i++){
            if(i == 0) cout << sort[roundVisit[i]];
            else cout << " " << sort[roundVisit[i]];
        }
        cout << endl;
        return;
    }

    for(int i = start ; i < k ; i++){ //O(n^2/2) (1~k, 2~k, 3~k, ...), stops when touch the limit k
        roundVisit[now] = i;
        printComb(sort, k, now + 1, i + 1);
    }
}

int main(){
    int k;
    cin >> k;
    int sort[k];

    for(int i = 0 ; i < k ; i++) cin >> sort[i];

    printComb(sort, k, 0, 0);

    return 0;
}