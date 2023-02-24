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

template <typename T> set<T> inputSet(int size) {
    set<T> sett;
    for (int i = 0; i < size; i++) {
        T val;
        cin >> val;
        sett.insert(val);
    }
    return sett;
}

template <typename T, typename K> void upsert(map<T, K> &m, T key, K val) {
    if (m.find(key) == m.end()) {
        m[key] = val;
    } else {
        m[key] += val;
    }
}

int main() {
    int size;
    cin >> size;
    map<int, int> mmap;
    for (int i = 0; i < size; i++) {
        int m;
        cin >> m;
        upsert(mmap, m, 1);
    }
    int min = INT_MAX;
    int restaurant = 0;
    for (const auto& [key, value] : mmap) {
        cout << key << " " << value << endl;
        if (value < min) {
            restaurant = key;
            min = value;
        }
    }
    cout << restaurant;
    return 0;
}

int set26() {
    int size;
    cin >> size;
    set<string> sett;
    for (int i = 0; i < size; i++) {
        int m;
        string s1, s2;
        cin >> m >> s1 >> s2;
        sett.insert(s1 + s2);
    }
    cout << sett.size();
    return 0;
}

int set25() {
    int size;
    cin >> size;
    set<int> sett;
    for (int i = 0; i < size; i++) {
        int m;
        cin >> m;
        cout << (sett.insert(m).second ? "NO\n" : "YES\n");
    }
    return 0;
}

int set24() {
    int workers, days;
    set<int> sett;
    cin >> workers >> days;
    for (int i = 0; i < workers; i++) {
        int from, to;
        cin >> from >> to;
        for (int d = from; d < to; d++) {
            sett.insert(d);
        }
    }
    cout << ((sett.size() == days) ? "YES" : "NO");
    return 0;
}

int set23() {
    int x, y;
    set<int> sett;
    cin >> x >> y;
    int n = 0;
    for (int xp = 0; xp < x; xp++) {
        for (int yp = 0; yp < y; yp++) {
            int m;
            cin >> m;
            if (!sett.insert(m).second) {
                n++;
            }
        }
    }
    cout << n;
    return 0;
}

int set22() {
    string word;
    cin >> word;
    set<char> sett;
    int n = 0;
    for (char ch : word) {
        if (!isdigit(ch)) {
            n++;
        } else if (!sett.insert(ch).second) {
            n++;
        }
        if (n > 2) {
            cout << "NO";
            return 0;
        }
    }
    if (sett.size() + n <= 9) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}

int set21() {
    string word;
    cin >> word;
    set<char> sett;
    int n = 0;
    for (char ch : word) {
        if (!isalpha(ch)) {
            n++;
        } else if (!sett.insert(ch).second) {
            n++;
        }
    }
    cout << n;
    return 0;
}

int set20() {
    int x, y;
    set<int> sett;
    cin >> x >> y;
    for (int xp = 0; xp < x; xp++) {
        for (int yp = 0; yp < y; yp++) {
            int m;
            cin >> m;
            if (xp == 0 || yp == 0 || xp == x - 1 || yp == y - 1) {
                sett.insert(m);
            }
        }
    }
    cout << sett.size();
    return 0;
}

int set19() {
    int size, min_range, max_range;
    cin >> size >> min_range >> max_range;
    set<int> sett = inputSet<int>(size);
    int n = 0;
    for (int val : sett) {
        if (min_range <= val && val < max_range) {
            n++;
        }
    }
    cout << n;
    return 0;
}

int set18() {
    int size;
    cin >> size;
    set<int> seen = inputSet<int>(size);
    cout << seen.size();
    return 0;
}