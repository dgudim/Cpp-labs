#include "../genericFunctions.h"
#include <thread>

using namespace std;

long hash__(string str) {
    uint32_t hash = 3323198485ul;
    for (char ch : str) {
        hash ^= ch;
        hash *= 0x5bd1e995;
        hash ^= hash >> 15;
    }
    return hash;
}

vector<string> strs;
vector<long> hashes;

int main() {

    auto test = [](char a) {
        long curr_hash = 0;
        string curr_str;
        string temp_str;
        for (char b = 'a'; b <= 'z'; b++) {
            for (char c = 'a'; c <= 'z'; c++) {
                for (char d = 'a'; d <= 'z'; d++) {
                    curr_str.clear();
                    curr_str.push_back(a);
                    curr_str.push_back(b);
                    curr_str.push_back(c);
                    curr_str.push_back(d);
                    curr_hash = hash__(curr_str);
                    if (find(hashes.begin(), hashes.end(), curr_hash) == hashes.end()) {
                        hashes.push_back(curr_hash);
                        strs.push_back(curr_str);
                    } else {
                        temp_str = strs.at(distance(hashes.begin(), find(hashes.begin(), hashes.end(), curr_hash)));
                        cout << "Name clash! " << curr_str << " and " << temp_str << endl;
                        cout << hash__(curr_str) << endl;
                        cout << hash__(temp_str) << endl;
                        cout << hashes.size() << " successfully hashed" << endl;
                        _getch();
                        exit(1);
                    }
                }
            }
        }
    };

    for (char a = 'a'; a <= 'z'; a++) {
        thread thread_object = thread(test, a);
        cout << "." << flush;
    }
   
    cout << "All good :) " << hashes.size() << endl;
    _getch();
    return 0;
}