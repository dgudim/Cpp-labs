#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits.h>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename K> void upsert(map<T, K> &m, T key, K val) {
    if (m.find(key) == m.end()) {
        m[key] = val;
    } else {
        m[key] += val;
    }
}

template <typename T, typename K> map<T, K> input_and_upsert(int size, K upsert_val) {
    map<T, K> mapp;
    for (int i = 0; i < size; i++) {
        T key;
        cin >> key;
        upsert(mapp, key, upsert_val);
    }
    return mapp;
}

template <typename T> void outputVector(const vector<T> &vec, const char *sep = " ") {
    if (!vec.empty()) {
        cout << vec[0];
        for (int i = 1; i < vec.size(); i++) {
            cout << sep << vec[i];
        }
    }
}

int main() {
    
    return 0;
}

int maps13() {
    int size;
    cin >> size;
    map<int, int> mmap = input_and_upsert<int, int>(size, 1);
    int max_number = INT_MIN;
    for (const auto &[number, repeats] : mmap) {
        if (repeats > 2 && number > max_number) {
            max_number = number;
        }
    }
    if (max_number == INT_MIN) {
        cout << "NO";
    } else {
        cout << max_number;
    }
    return 0;
}

int maps12() {
    string word;
    cin >> word;
    map<char, int> mapp;
    for (char ch : word) {
        upsert(mapp, ch, 1);
    }
    int count = 0;
    for (const auto &[key, value] : mapp) {
        if (value > 2) {
            count++;
        }
    }
    cout << count;
    return 0;
}

int maps11_02() {
    int size;
    cin >> size;
    map<string, string> mapp;
    for (int i = 0; i < size; i++) {
        string key, value;
        cin >> key >> value;
        mapp.insert(make_pair(key, value));
    }
    for (const auto &[key, value] : mapp) {
        if (mapp.find(value) != mapp.end() && mapp.at(value) == key) {
            cout << "YES";
            return 0;
        }
    }
    cout << "NO";
    return 0;
}

int maps10_11(bool print_word) {
    int size;
    cin >> size;
    map<string, int> mmap = input_and_upsert<string, int>(size, 1);
    int max = INT_MIN;
    string word = "";
    for (const auto &[key, value] : mmap) {
        if (value > max) {
            max = value;
            word = key;
        }
    }
    if (print_word) {
        cout << word;
    } else {
        cout << max;
    }
    return 0;
}

int maps8() {
    int size;
    cin >> size;
    map<int, int> mmap = input_and_upsert<int, int>(size, 1);
    ;
    int requests;
    cin >> requests;
    vector<int> answ(requests);
    for (int i = 0; i < requests; i++) {
        int mark;
        cin >> mark;
        upsert(mmap, mark, 0);
        answ[i] = mmap.at(mark);
    }
    outputVector(answ);
    return 0;
}