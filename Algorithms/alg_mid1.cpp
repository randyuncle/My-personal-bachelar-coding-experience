#include <iostream>

using namespace std;

//fib-like?
long long int count(int n){
    long long int c[n + 1]; c[0] = 0;
    for(int i = 1 ; i <= n ; i++){
        if(i > 2) c[i] = c[i - 1] + c[i - 2];
        else if(i == 1) c[i] = 1;
        else c[i] = 2; //i == 2
    }

    return c[n];
}

int main(){
    int n;
    cin >> n;

    cout << count(n);
    return 0;
}