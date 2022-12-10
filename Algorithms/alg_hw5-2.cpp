#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#define MAX 500000

using namespace std;

vector <string> wineNames;
unordered_map <string, int> chk;

void merge(int front, int mid, int end) {
    vector<string> left(wineNames.begin() + front, wineNames.begin() + mid + 1);
    vector<string> right(wineNames.begin() + mid + 1, wineNames.begin() + end + 1);

    int indL = 0, indR = 0, i = front;
    while ((indL < left.size()) && (indR < right.size())) {
        string leftName = left[indL], rightName = right[indR];
        if (chk[leftName] < chk[rightName]) wineNames[i++] = left[indL++];
        else if (chk[leftName] > chk[rightName]) wineNames[i++] = right[indR++];
        else {
            if(leftName > rightName) wineNames[i++] = left[indL++];
            else wineNames[i++] = right[indR++];
        }
    }

    while (indL < left.size()) wineNames[i++] = left[indL++];
    while (indR < right.size()) wineNames[i++] = right[indR++];

    return;
}
void mergeSort(int front, int end) {
    if (front < end) {
        int mid = (front + end) / 2;
        mergeSort(front, mid);
        mergeSort( mid + 1, end);
        merge(front, mid, end);
    }

    return;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

	int n, k;
	cin >> n >> k;

	for (int i = 0; i < n; i++) {
		string wine;
		cin >> wine;

		if (!chk.count(wine)) {
			chk.insert(pair<string, int>(wine, 1));
			wineNames.push_back(wine);
		}
		else chk[wine] += 1;
	}

    mergeSort(0, wineNames.size() - 1);

    cout << wineNames[wineNames.size() - k];
    return 0;
}