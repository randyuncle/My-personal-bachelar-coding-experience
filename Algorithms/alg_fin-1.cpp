#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#define lli long long int

using namespace std;

lli ans = 0;
int v = 0;

struct node {
    int from, to, d;
};
node edge[800010];

typedef struct subset {
    int parent;
    int rank;
}
subset;
subset subsets[500010];

bool cmp(node a, node b){
    return a.d > b.d;
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
        Union(edge[i].from, edge[i].to, edge[i].d);
}

int main() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++)
        cin >> edge[i].from >> edge[i].to >> edge[i].d;
    
    for(int i = 0 ; i < n ; i++){
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    sort(edge, edge + m, cmp);
    mst(n, m);
    cout << ans;
    return 0;
}