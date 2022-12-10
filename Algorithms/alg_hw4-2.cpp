#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

pair<char, int> element[129], buffer[129];

void merge(int front, int mid, int end){
    int indL = front, indR = mid + 1, i = front;
    while((indL <= mid ) && (indR <= end)){
        if(element[indL].second > element[indR].second) buffer[i++] = element[indL++];
        else if(element[indL].second < element[indR].second) buffer[i++] = element[indR++];
        else{
            if(element[indL].first < element[indR].first) buffer[i++] = element[indL++];
            else buffer[i++] = element[indR++];
        }
    }

    while(indL <= mid) buffer[i++] = element[indL++];
    while(indR <= end) buffer[i++] = element[indR++];

    for(i = front ; i <= end ; i++) element[i] = buffer[i];
    return;
}
void mergeSort(int front, int end){
    if(front < end){
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort(mid + 1, end);
        merge(front, mid, end);
    }

    return;
}

int main(){
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);

    string str;
    cin >> str;

    for(int i = 0 ; i < 128 ; i++) element[i].first = (char)i, element[i].second = 0;
    for(char& c : str) ++element[c].second;

    mergeSort(0, 127);

    for(int i = 0 ; i < 128 ; i++){
        if(element[i].second == 0)
            break;
        for(int j = 0 ; j < element[i].second ; j++) cout << element[i].first;
    }

    return 0;
}