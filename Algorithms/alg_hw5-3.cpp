#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

typedef tree<int, null_type,less_equal<int>, rb_tree_tag,tree_order_statistics_node_update> ordered_set;

int main(){
    int n, m, k;
    cin >> n >> m >> k;

    vector <int> wine;
    ordered_set picks;

    for (int i = 0; i < n; i++) {
        int input;
        cin >> input;
        wine.push_back(input);
        picks.insert(input);
        if(i >= m-1) {
            if(i >= m) picks.erase(picks.upper_bound(wine[i - m]));
            cout << *picks.find_by_order(k-1) << " ";
        }
    } 

    return 0;
}