#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

vector<int> inputVec(int size) {
    vector<int> vec(size);
    for(int i = 0; i < size; i++) {
        cin >> vec[i];
    }
    return vec;
}

void outputVector(const vector<int>& vec, const char* sep = " ") {
    if(vec.size()) {
        cout << vec[0];
        for(int i = 1; i < vec.size(); i++){
            cout << sep << vec[i];
        }
    }
}

int main() {
    int size;
    cin >> size;
    vector<int> marks(size);
    int peter_index;
    for(int i = 0; i < size; i++) {
        string name;
        cin >> name >> marks[i];
        if(name == "Peter") {
            peter_index = i;
        }
    }

    return 0;
}

int vector5() {
    int size;
    cin >> size;
    vector<int> A(size);
    vector<int> B;
    for(int i = 0; i < size; i++) {
        A[i] = i + 1;
    }
    while(true) {
        // take card from A and put into B
        B.push_back(A.at(0));
        // remove from A
        A.erase(A.begin());

        if(A.size() == 0) {
            outputVector(B, ",");
            return 0;
        }

        // put element from start of A to the end of A
        A.push_back(A.at(0));
        A.erase(A.begin());

        if(A.size() == 0) {
            outputVector(B, ",");
            return 0;
        }
    }
    return 0;
}

size_t vhash(vector<int> const& vec) {
  size_t seed = vec.size();
  for(auto& i : vec) {
    seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

int vector4() {
    int peter_size, jhon_size;
    cin >> peter_size >> jhon_size;
    vector<int> peter = inputVec(peter_size);
    vector<int> jhon = inputVec(jhon_size);

    vector<size_t> seen;

    int turns = 0;
    while (true) {
        int peter_card = peter.at(0);
        peter.erase(peter.begin());
        int jhon_card = jhon.at(0);
        jhon.erase(jhon.begin());
        if(peter_card > jhon_card) {
            peter.push_back(jhon_card);
            peter.push_back(peter_card);
        } else if (jhon_card > peter_card) {
            jhon.push_back(peter_card);
            jhon.push_back(jhon_card);
        } else {
            cout << "Both losers";
            return 0;
        }
        turns ++;
        size_t vector_hash = vhash(peter) + vhash(jhon);
        if(peter.size() == 0) {
            cout << turns << " Jhon";
            return 0;
        } else if (jhon.size() == 0) {
            cout << turns << " Peter";
            return 0;
        } else if (find(seen.begin(), seen.end(), vector_hash) != seen.end()) {
            cout << "Both losers";
            return 0;
        }
        seen.push_back(vector_hash);
    }
    return 0;
}

int vector3() {
    int size;
    cin >> size;
    vector<int> vec = inputVec(size);
    vector<int> positive;
    vector<int> negative;
    for(const auto& elem: vec) {
        if(elem < 0) {
            negative.push_back(elem);
        } else {
            positive.push_back(elem);
        }
    }
    positive.insert(
      positive.end(),
      make_move_iterator(negative.begin()),
      make_move_iterator(negative.end())
    );
    outputVector(positive);
    return 0;
}

int vector2() {
    int size;
    int queries;
    cin >> size >> queries;
    vector<int> vec = inputVec(size);
    for(int i = 0; i < queries; i++) {
        string command;
        int index;
        cin >> command >> index;
        if(command == "sum") {
            int sum = 0;
            for(int s = 0; s < index; s++) {
                sum += vec.at(s);
            }
            cout << sum << "\n";
        } else if (command == "del") {
            vec.erase(vec.begin() + index - 1);
        } else if (command == "pap") {
            vec.insert(vec.begin() + index, index * 2);
        }
    }
    return 0;
}

int vector1() {
    int size;
    int queries;
    cin >> size >> queries;
    vector<int> vec = inputVec(size);
    for(int i = 0; i < queries; i++) {
        int index;
        cin >> index;
        vec.erase(vec.begin() + index - 1);
    }
    outputVector(vec);
    return 0;
}
