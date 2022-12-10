#include <iostream>
#include <vector>

using namespace std;

vector <int> array(2000000);

void maxSubArr(int n){
    int last = 0, maxSum = array[0], first = 1, tail = 1, nfirst = 0, setTail = 0;
    for(int i = 0 ; i < n ; i++){
        last = max(last + array[i], array[i]);
        if(last < 0) nfirst = i + 2, last = 0;
        else if(last > maxSum && nfirst) first = nfirst, nfirst = 0;

        maxSum = max(last, maxSum);
        if(maxSum > last && !setTail) tail = i, setTail++;
        else if(maxSum == last) setTail = 0;
    }
    if(first > tail) tail = n;
    cout << maxSum << " " << first << " " << tail ;
}

int main(){
    int n;
    cin >> n;

    for(int i = 0 ; i < n ; i++) cin >> array[i];
    maxSubArr(n);
    return 0;
}