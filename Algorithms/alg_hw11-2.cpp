#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#define lli long long int

using namespace std;

lli ans = 0;
int v = 0;

struct node {
    int id[2], d;
};
node edge[2000*2000 + 10];

typedef struct subset {
    int parent;
    int rank;
}
subset;
subset subsets[2010];

bool cmp(node a, node b){
    return a.d < b.d;
}

int find(int i){
    if(subsets[i].parent != i)
        subsets[i].parent = find(subsets[i].parent);
    return subsets[i].parent;
}

void Union(int i, int j, int w){
    int aRoot = find(i);
    int bRoot = find(j);

    if(aRoot != bRoot){
        //compare the ranks
        if(subsets[aRoot].rank > subsets[bRoot].rank)
            subsets[bRoot].parent = aRoot;
        else if(subsets[aRoot].rank < subsets[bRoot].rank)
            subsets[aRoot].parent = bRoot;
        else{
            subsets[bRoot].parent = aRoot;
            subsets[aRoot].rank++;
        }
        ans += w;
        v++;
    }
}

void mst(int n, int count) {
    for (int i = 0; i < count && v < n - 1; i++) 
        Union(edge[i].id[0], edge[i].id[1], edge[i].d);
}

int main() {
    int n, count = 0;
    string str[2010];
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> str[i];
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
    cin >> str[n];

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++){
            int diff = 0;
            for(int k = 0 ; k < 7 ; k++)
                if(str[i][k] != str[j][k]) diff++;
            edge[count].id[0] = i; 
            edge[count].id[1] = j; 
            edge[count++].d = diff;
        }
    sort(edge, edge + count, cmp);
    mst(n, count);
    cout << "The highest possible quality is 1/" << ans << ".";
    return 0;
}