#include <iostream>
#include <vector>
#define lli long long int

using namespace std;

pair<int, int> point[1000];
lli ans = 0;

struct node {
    int id[2], d;
};
vector<node> edge(1000 * 1000 + 10);

typedef struct subset 
{
    int parent;
    int rank;
}
subset;
subset subsets[1000];

void merge(vector<node>& sort, int front, int mid, int end) {
    vector<node> left(sort.begin() + front, sort.begin() + mid + 1);
    vector<node> right(sort.begin() + mid + 1, sort.begin() + end + 1);

    node max;
    max.id[0] = -1, max.id[1] = -1, max.d = INT32_MAX;
    left.push_back(max);
    right.push_back(max);

    int indL = 0, indR = 0;
    for (int i = front; i <= end; i++) {
        if (left[indL].d <= right[indR].d) sort[i] = left[indL++];
        else sort[i] = right[indR++];
    }

    return;
}
void mergeSort(vector<node>& sort, int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(sort, front, mid);
        mergeSort(sort, mid + 1, end);
        merge(sort, front, mid, end);
    }

    return;
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
    }
}

lli mst(int n) {
    for (int i = 0; i < n; i++) 
        Union(edge[i].id[0], edge[i].id[1], edge[i].d);
    return ans;
}

int main() {
    int n, count = 0;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> point[i].first >> point[i].second;
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            edge[count].id[0] = i, edge[count].id[1] = j, edge[count++].d = abs(point[j].first - point[i].first) + abs(point[j].second - point[i].second);
    mergeSort(edge, 0, count - 1);
    cout << mst(count);
    return 0;
}