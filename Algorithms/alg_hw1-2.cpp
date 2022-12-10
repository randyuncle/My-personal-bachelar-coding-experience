#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
//#define MAX 250000

using namespace std;

/**
 * @brief the stacks
 * hint : its pop WON'T return any value 
 */
stack <int> stacks;
/**
 * Using to make a r-b tree of the sets in a node of a stack (O(logn)). 
 * Can use the array-like type to access the data of the current key 
 */
map <set<int>, int> nullId; 
//to save set the sets ( array-like ?
vector <set<int>> sets;

/**
 * it done in two method:
 * first : check if the sets is in the sorted map in rb-tree mode
 * second : if no, then push the sets into the vector, which saves the sets that input.
 * 
 * @param input 
 * @return int (the number of the sets in one node of vector)
 */
int findSets(set<int> input){
    if(nullId.count(input)) return nullId[input];

    sets.push_back(input);
    nullId[input] = sets.size() - 1;
    return nullId[input];
}

void push(){
    stacks.push(findSets(set<int>()));
    return;
}

void dup(){
    stacks.push(stacks.top());
    return;
}

void Union(){
    set<int> first = sets[stacks.top()];
    stacks.pop();

    set<int> second = sets[stacks.top()];
    stacks.pop();

    set<int> input;
    set_union(first.begin(), first.end(), second.begin(), second.end(), inserter(input, input.begin()));

    stacks.push(findSets(input));
    return;
}

void intersect(){
    set<int> first = sets[stacks.top()];
    stacks.pop();

    set<int> second = sets[stacks.top()];
    stacks.pop();

    set<int> input;
    set_intersection(first.begin(), first.end(), second.begin(), second.end(), inserter(input, input.begin()));

    stacks.push(findSets(input));
    return;
}

void add(){
    set<int> first = sets[stacks.top()];
    stacks.pop();

    set<int> second = sets[stacks.top()];
    stacks.pop();

    second.insert(findSets(first));
    stacks.push(findSets(second));
    return;
}

int main(){
    int round;
    cin >> round;

    for(int i = 0 ; i < round ; i++){
        string command;
        cin >> command;

        if(command == "PUSH") push();
        else if(command == "DUP") dup();
        else if(command == "UNION") Union();
        else if(command == "INTERSECT") intersect();
        else if(command == "ADD") add();

        cout << sets[stacks.top()].size() << endl;
    }

    return 0;
}