#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#define MAX 100000

using namespace std;

int table[MAX][26];

void initializeMap(string s){
    memset(table, 0, sizeof(table));
    for(int i = s.length() - 1 ; i >= 0 ; i--){
        for(int j = 0 ; j < 26 ; j++) table[i][j] = table[i + 1][j];
        table[i][s[i] - 'a'] = i + 1;
    }
    return;
}

int main(){
    int n;
    cin >> n;

    for(int i = 0 ; i < n ; i++){
        string s, t;
        cin >> s >> t;
        initializeMap(s);

        int pivot = 0, chg = 0;
        for(int j = 0 ; j < t.length() ; j++){
            int val = t[j] - 'a';
            if(!table[pivot][val]) pivot = 0;
            if(!pivot){
                chg++;
                if(!table[pivot][val]){
                    chg = -1;
                    break;
                }
            }
            pivot = table[pivot][val];
        }

        cout << chg << endl;
    }

    return 0;
}