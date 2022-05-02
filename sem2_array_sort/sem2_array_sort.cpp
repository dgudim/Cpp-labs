#include "../genericFunctions.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

struct ArrayStruct {
    ArrayStruct(int key_ = 0, string data_ = "") {
        key = key_;
        data_ = data_;
    }
    int key;
    string data;
};

typedef vector<ArrayStruct>(*SortFunction)(vector<ArrayStruct>, unsigned int&, unsigned int&);

bool checkIfSorted(vector<ArrayStruct> array) {
    for (int i = 0; i < array.size() - 1; i++) {
        if (array[i].key > array[i + 1].key) {
            return false;
        }
    }
    return true;
}

vector<ArrayStruct> bubbleSort(vector<ArrayStruct> orig, unsigned int& iterations, unsigned int& changes) {
    unsigned int n = orig.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (orig[i].key > orig[j].key) {
                swap(orig[i], orig[j]);
                changes++;
            }
            iterations++;
        }
    }
    return orig;
}

vector<ArrayStruct> directSelectionSort(vector<ArrayStruct> orig, unsigned int& iterations, unsigned int& changes) {
    unsigned int n = orig.size();
    int temp_index;
    for (int i = 0; i < n - 1; i++) {
        temp_index = i;
        for (int j = i + 1; j < n; j++) {
            if (orig[j].key < orig[temp_index].key) {
                temp_index = j;
            }
            iterations++;
        }
        swap(orig[temp_index], orig[i]);
        changes++;
    }
    return orig;
}

vector<ArrayStruct> shakerSort(vector<ArrayStruct> orig, unsigned int& iterations, unsigned int& changes) {
    unsigned int n = orig.size();
    for (int i = 0; i < n;) {
        for (int j = i + 1; j < n; j++) {
            if (orig[j].key < orig[j - 1].key) {
                swap(orig[j], orig[j - 1]);
                changes++;
            }
            iterations++;
        }
        n--;
        for (int k = n - 1; k > i; k--) {
            if (orig[k].key < orig[k - 1].key) {
                swap(orig[k], orig[k - 1]);
                changes++;
            }
            iterations++;
        }
        i++;
    }
    return orig;
}

int partition(vector<ArrayStruct>& orig, int low, int high, unsigned int& iterations, unsigned int& changes) {
    int pivot = orig[high].key;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (orig[j].key <= pivot) {
            i++;
            swap(orig[i], orig[j]);
            changes++;
        }
        iterations++;
    }
    swap(orig[i + 1], orig[high]);
    changes++;
    return (i + 1);
}

void quickSort(vector<ArrayStruct>& orig, int low, int high, unsigned int& iterations, unsigned int& changes) {
    if (low < high) {
        int pi = partition(orig, low, high, iterations, changes);
        quickSort(orig, low, pi - 1, iterations, changes);
        quickSort(orig, pi + 1, high, iterations, changes);
    }
}

vector<ArrayStruct> quickSortWrapper(vector<ArrayStruct> orig, unsigned int& iterations, unsigned int& changes) {
    quickSort(orig, 0, orig.size() - 1, iterations, changes);
    return orig;
}

vector<ArrayStruct> shellSort(vector<ArrayStruct> orig, unsigned int& iterations, unsigned int& changes) {
    unsigned int n = orig.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            
            ArrayStruct temp = orig[i];

            int j;
            for (j = i; j >= gap && orig[j - gap].key > temp.key; j -= gap) {
                orig[j] = orig[j - gap];
                changes++;
                iterations++;
            }

            orig[j] = temp;
            changes++;
            iterations++;
        }
    }
    return orig;
}

Vector2 benchmarkSort(SortFunction sortFunction, string name, vector<ArrayStruct>& orig) {
    unsigned int iterations = 0, changes = 0;
    auto start = high_resolution_clock::now();
    vector<ArrayStruct> sorted = sortFunction(orig, iterations, changes);
    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
    bool sorted_s = checkIfSorted(sorted);
    if (sorted_s) {
        coutWithColor(colors::LIGHT_GREEN, "Результат для ");
        coutWithColor(colors::CYAN, name);
        coutWithColor(colors::LIGHT_GREEN, " на ");
        coutWithColor(colors::PURPLE, to_string(orig.size()));
        coutWithColor(colors::LIGHT_GREEN, " элементах: ");
        coutWithColor(colors::YELLOW, to_string(iterations));
        coutWithColor(colors::LIGHT_GREEN, " итераций, ");
        coutWithColor(colors::YELLOW, to_string(changes));
        coutWithColor(colors::LIGHT_GREEN, " изменений массива, ");
        coutWithColor(colors::YELLOW, doubleToString(duration.count() / 1000.0));
        coutWithColor(colors::LIGHT_GREEN, "ms\n");
    } else {
        coutWithColor(colors::LIGHT_RED, "Ошибка сортировки для " + name + "!\n");
    }
    return { iterations, changes };
}

int main() {

    vector<ArrayStruct> orig;
    for (int i = 0; i < 1000; i++) {
        orig.push_back(ArrayStruct(rand(), ""));
    }
    Vector2 bubble = benchmarkSort(bubbleSort, "сортировки пузырьком", orig);
    Vector2 shaker = benchmarkSort(shakerSort, "шейкерной сортировки", orig);
    Vector2 directSelection = benchmarkSort(directSelectionSort, "сортировки прямым выбором", orig);
    Vector2 quick = benchmarkSort(quickSortWrapper, "быстрой сортировки", orig);
    Vector2 shell = benchmarkSort(shellSort, "сортировки Шелла", orig);

    vector<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);

    printBarChart(values, 16, 4, 2);

    coutWithColor(colors::LIGHTER_BLUE, "Нажмите любую кнопку, чтобы выйти\n");
    _getch();

    return 0;
}