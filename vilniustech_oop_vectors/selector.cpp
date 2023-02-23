#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T> vector<T> inputVec(int size) {
    vector<T> vec(size);
    for (int i = 0; i < size; i++) {
        cin >> vec[i];
    }
    return vec;
}

template <typename T1, typename T2> vector<pair<T1, T2>> inputPairVec(int size) {
    vector<pair<T1, T2>> vec(size);
    for (int i = 0; i < size; i++) {
        T1 first;
        T2 second;
        cin >> first >> second;
        vec[i] = make_pair(first, second);
    }
    return vec;
}

template <typename T> void outputVector(const vector<T> &vec, const char *sep = " ") {
    if (!vec.empty()) {
        cout << vec[0];
        for (int i = 1; i < vec.size(); i++) {
            cout << sep << vec[i];
        }
    }
}

template <typename T> bool vectorContains(const vector<T> &vec, T val) {
    return find(vec.begin(), vec.end(), val) != vec.end();
}

typedef long long verylong;

bool try_next_num(const vector<verylong> &nums, vector<verylong> &list) {
    if (list.size() == nums.size()) {
        return true;
    }
    verylong num = list.back();
    verylong possible3 = num / 3L;
    verylong possiblex2 = num * 2L;
    if (num % 3 == 0 && vectorContains(nums, possible3) && !vectorContains(list, possible3)) {
        int size = list.size();
        list.push_back(possible3);
        if (try_next_num(nums, list)) {
            return true;
        }
        list.resize(size);
    }
    if (vectorContains(nums, possiblex2) && !vectorContains(list, possiblex2)) {
        int size = list.size();
        list.push_back(possiblex2);
        if (try_next_num(nums, list)) {
            return true;
        }
        list.resize(size);
    }
    return false;
}

int main() {
    return 0;
}

int vector25() {
    int size;
    cin >> size;
    vector<verylong> nums = inputVec<verylong>(size);
    for (verylong num : nums) {
        vector<verylong> tried;
        tried.push_back(num);
        if (try_next_num(nums, tried)) {
            outputVector(tried);
            return 0;
        }
    }
    return 0;
}

int vector30() {
    int size;
    cin >> size;
    vector<string> food_queue = inputVec<string>(size);
    int customers;
    cin >> customers;
    vector<pair<string, string>> customer_queue = inputPairVec<string, string>(customers);
    while (!customer_queue.empty()) {
        string food_req = customer_queue.front().second;
        if (food_queue.front() == food_req) {
            food_queue.erase(food_queue.begin());
            customer_queue.erase(customer_queue.begin());
        } else if (food_queue.back() == food_req) {
            food_queue.pop_back();
            customer_queue.erase(customer_queue.begin());
        } else {
            if (!vectorContains(food_queue, food_req)) {
                food_queue.push_back(food_req);
            }
            customer_queue.push_back(customer_queue.front());
            customer_queue.erase(customer_queue.begin());
        }
        outputVector(food_queue);
        cout << "\n";
    }
    return 0;
}

int vector29() {
    int size;
    cin >> size;
    vector<string> storage = inputVec<string>(size);
    int requests;
    cin >> requests;
    string name, c_req;
    int fullfilments = 0;
    for (int i = 0; i < requests; i++) {
        cin >> name >> c_req;
        auto pos = find(storage.begin(), storage.end(), c_req);
        if (pos != storage.end()) {
            fullfilments++;
            storage.erase(pos);
        }
    }
    cout << fullfilments;
    return 0;
}

int vector6() {
    int size;
    cin >> size;
    vector<pair<int, string>> marks_names = inputPairVec<int, string>(size);
    int min = 0;
    while (true) {
        marks_names.at(0).first++;
        min++;
        if (marks_names.at(0).first >= 5) {
            if (marks_names.at(0).second == "Peter") {
                cout << min;
                return 0;
            }
        } else {
            marks_names.push_back(marks_names.at(0));
        }
        marks_names.erase(marks_names.begin());
    }
    return 0;
}

int vector5() {
    int size;
    cin >> size;
    vector<int> A(size);
    vector<int> B;
    for (int i = 0; i < size; i++) {
        A[i] = i + 1;
    }
    while (true) {
        // take card from A and put into B
        B.push_back(A.at(0));
        // remove from A
        A.erase(A.begin());

        if (A.empty()) {
            outputVector(B, ",");
            return 0;
        }

        // put element from start of A to the end of A
        A.push_back(A.at(0));
        A.erase(A.begin());

        if (A.empty()) {
            outputVector(B, ",");
            return 0;
        }
    }
    return 0;
}

size_t vhash(vector<int> const &vec) {
    size_t seed = vec.size();
    for (auto &i : vec) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

int vector4() {
    int peter_size, jhon_size;
    cin >> peter_size >> jhon_size;
    vector<int> peter = inputVec<int>(peter_size);
    vector<int> jhon = inputVec<int>(jhon_size);

    vector<size_t> seen;

    int turns = 0;
    while (true) {
        int peter_card = peter.at(0);
        peter.erase(peter.begin());
        int jhon_card = jhon.at(0);
        jhon.erase(jhon.begin());
        if (peter_card > jhon_card) {
            peter.push_back(jhon_card);
            peter.push_back(peter_card);
        } else if (jhon_card > peter_card) {
            jhon.push_back(peter_card);
            jhon.push_back(jhon_card);
        } else {
            cout << "Both losers";
            return 0;
        }
        turns++;
        size_t vector_hash = vhash(peter) + vhash(jhon);
        if (peter.empty()) {
            cout << turns << " Jhon";
            return 0;
        } else if (jhon.empty()) {
            cout << turns << " Peter";
            return 0;
        } else if (vectorContains(seen, vector_hash)) {
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
    vector<int> vec = inputVec<int>(size);
    vector<int> positive;
    vector<int> negative;
    for (const auto &elem : vec) {
        if (elem < 0) {
            negative.push_back(elem);
        } else {
            positive.push_back(elem);
        }
    }
    positive.insert(positive.end(), make_move_iterator(negative.begin()),
                    make_move_iterator(negative.end()));
    outputVector(positive);
    return 0;
}

int vector2() {
    int size;
    int queries;
    cin >> size >> queries;
    vector<int> vec = inputVec<int>(size);
    for (int i = 0; i < queries; i++) {
        string command;
        int index;
        cin >> command >> index;
        if (command == "sum") {
            int sum = 0;
            for (int s = 0; s < index; s++) {
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
    vector<int> vec = inputVec<int>(size);
    for (int i = 0; i < queries; i++) {
        int index;
        cin >> index;
        vec.erase(vec.begin() + index - 1);
    }
    outputVector(vec);
    return 0;
}
