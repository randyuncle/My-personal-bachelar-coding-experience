#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <string>
#include <vector>
#include <algorithm>
#define MAX 1009

using namespace std;

struct Union{
    int where;
    int locate;
};
vector <int> list[MAX];
struct Union place[MAX];

void replace(int curr, int blk){
    int top = list[curr].size() - 1;
    for(; list[curr][top] != blk ; top--){
        int origin = list[curr][top];
        list[origin].push_back(origin);
        list[curr].pop_back();
        place[origin].where = origin;
        place[origin].locate = list[origin].size() - 1;
    }
    return;
}

void displace(int from, int to, int a){
    int topF = list[from].size(), count = 0;
    for(int i = place[a].locate ; i < topF ; i++, count++){
        int transfer = list[from][i];
        list[to].push_back(transfer);
        place[transfer].where = to;
        place[transfer].locate = list[to].size() - 1;
    }
    list[from].resize(topF - count);
    return;
}

int main(){
    int block, round;
    cin >> block >> round;

    for(int i = 1 ; i <= block ; i++){
        list[i].push_back(i);
        place[i].where = i;
        place[i].locate = 0;
    }

    for(int i = 0 ; i < round ; i++){
        string command, choice;
        int a, b;
        cin >> command >> a >> choice >> b;

        int placeA = place[a].where, placeB = place[b].where;
        if(placeA != placeB){
            if((command == "move") && (choice == "onto")){
                replace(placeA, a);
                replace(placeB, b);
                displace(placeA, placeB, a);
            }else if((command == "move") && (choice == "over")){
                replace(placeA, a);
                displace(placeA, placeB, a);
            }else if((command == "pile") && (choice == "onto")){
                replace(placeB, b);
                displace(placeA, placeB, a);
            }else{
                displace(placeA, placeB, a);
            }
        }
    }

    for(int i = 1 ; i <= block ; i++){
        cout << i << ":";
        for(int j = 0 ; j < list[i].size() ; j++) cout << " " << list[i][j];
        cout << endl;
    }

    return 0;    
}