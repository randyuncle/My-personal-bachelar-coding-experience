#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#define MAX 100

using namespace std;

typedef struct info{
    char size;
    int place;
    int isEven;
}info;

info rocks[MAX];

int main(){
    int t;
    cin >> t;

    for(int i = 0 ; i < t ; i++){
        int n, smallcount = 0; int w;
        priority_queue<int> maxHeap;

        cin >> n >> w;
        for(int j = 0 ; j < n ; j++){
            char size; int place;
            cin >> size >> place;

            rocks[j].place = place;
            rocks[j].size = size;
            if(size == 's'){
                smallcount++;
                if(smallcount % 2 == 0) rocks[j].isEven = 1;
                else rocks[j].isEven = 0;
            }
            else smallcount = 0, rocks[j].isEven = -1;
        }

        int last = 0;
        for(int j = 0 ; j < n ; j++){
            if(rocks[j].size == 's'){
                if(rocks[j].isEven) continue;
                else maxHeap.push(rocks[j].place - last), last = rocks[j].place;
            }
            else maxHeap.push(rocks[j].place - last), last = rocks[j].place;
        }
        maxHeap.push(w - last);

        last = w;
        for(int j = n - 1 ; j >= 0 ; j--){
            if(rocks[j].size == 's'){
                if(rocks[j].isEven) maxHeap.push(last - rocks[j].place), last = rocks[j].place;
            }
            else maxHeap.push(last - rocks[j].place), last = rocks[j].place;
        }
        maxHeap.push(last - 0);

        cout << maxHeap.top() << endl;
    }
    return 0;
}