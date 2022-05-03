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

struct Metrics {
    unsigned int iterations;
    unsigned int changes;
    long int time;
    Metrics(unsigned int iterations_ = 0, unsigned int changes_ = 0, long int time_ = 0) {
        iterations = iterations_;
        changes = changes_;
        time = time_;
    }
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

Metrics benchmarkSort(SortFunction sortFunction, string name, vector<ArrayStruct>& orig) {
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
    return { iterations, changes, duration.count() };
}

void refillArray(vector<ArrayStruct>& values, int n) {
    values.clear();
    for (int i = 0; i < n; i++) {
        values.push_back(ArrayStruct(rand(), ""));
    }
}

int main() {
    int n = 25;
    vector<ArrayStruct> orig;
    vector<int> values;
    vector<string> titles;

    Metrics bubble, shaker, directSelection, quick, shell;

    vector<SortFunction> sortingFunctions = { 
        bubbleSort, 
        shakerSort, 
        directSelectionSort, 
        quickSortWrapper, 
        shellSort };
    vector<string> sortingFunctions_displayNames = { 
        "сортировки пузырьком", 
        "шейкерной сортировки",
        "сортировки прямым выбором",
        "быстрой сортировки",
        "сортировки Шелла"};
    vector<string> sortingFunctions_fileNames = {
        "bubble",
        "shaker",
        "directSelection",
        "quick",
        "shell" };

    while (true) {
        refillArray(orig, n);
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Текущий размер массива: " + to_string(n) + "\n");
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        switch (displaySelection(new string[4]{
                 "1.Ввести размер массива",
                 "2.Отсортировать и посмотреть результаты",
                 "3.График зависимости времени выполнения и измений массива от количества элементов",
                 "4.Выйти"
            }, 4)) {
        case 1:
            while (true) {
                n = inputData("Введите размер: ");
                if (n > 0) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "размер должен быть больше 0, повторите ввод\n");
            }
            break;
        case 2:

            values.clear();
            titles.clear();

            for (int i = 0; i < sortingFunctions.size(); i++) {

            }

            coutWithColor(mapToColor(0, 0, 5), "█");
            bubble = benchmarkSort(bubbleSort, "сортировки пузырьком", orig);
            coutWithColor(mapToColor(1, 0, 5), "█");
            shaker = benchmarkSort(shakerSort, "шейкерной сортировки", orig);
            coutWithColor(mapToColor(2, 0, 5), "█");
            directSelection = benchmarkSort(directSelectionSort, "сортировки прямым выбором", orig);
            coutWithColor(mapToColor(3, 0, 5), "█");
            quick = benchmarkSort(quickSortWrapper, "быстрой сортировки", orig);
            coutWithColor(mapToColor(4, 0, 5), "█");
            shell = benchmarkSort(shellSort, "сортировки Шелла", orig);

            values.clear();
            values.push_back(bubble.time);
            values.push_back(shaker.time);
            values.push_back(directSelection.time);
            values.push_back(quick.time);
            values.push_back(shell.time);

            titles.clear();
            titles.push_back(doubleToString(bubble.time / (float)1000) + "ms");
            titles.push_back(doubleToString(shaker.time / (float)1000) + "ms");
            titles.push_back(doubleToString(directSelection.time / (float)1000) + "ms");
            titles.push_back(doubleToString(quick.time / (float)1000) + "ms");
            titles.push_back(doubleToString(shell.time / (float)1000) + "ms");

            coutWithColor(colors::LIGHT_PURPLE, "\nВремя выполнения\n");
            printBarChart(values, titles, 64, 8, 2);

            values.clear();
            values.push_back(bubble.changes);
            values.push_back(shaker.changes);
            values.push_back(directSelection.changes);
            values.push_back(quick.changes);
            values.push_back(shell.changes);

            titles.clear();
            titles.push_back(to_string(bubble.changes));
            titles.push_back(to_string(shaker.changes));
            titles.push_back(to_string(directSelection.changes));
            titles.push_back(to_string(quick.changes));
            titles.push_back(to_string(shell.changes));

            coutWithColor(colors::LIGHT_PURPLE, "Количество изменений массива\n");
            printBarChart(values, titles, 64, 8, 2);

            coutWithColor(colors::LIGHTER_BLUE, "Нажмите любую кнопку, чтобы вернуться в меню\n");
            _getch();
            break;
        case 3:
            for (int i = 10; i < 1500; i += 4) {
                refillArray(orig, i);
                bubble = benchmarkSort(bubbleSort, "сортировки пузырьком", orig);
                shaker = benchmarkSort(shakerSort, "шейкерной сортировки", orig);
                directSelection = benchmarkSort(directSelectionSort, "сортировки прямым выбором", orig);
                quick = benchmarkSort(quickSortWrapper, "быстрой сортировки", orig);
                shell = benchmarkSort(shellSort, "сортировки Шелла", orig);

                system(("echo '" + to_string(i) + " " + to_string(bubble.time / (double)1000) + "' >> bubble").c_str());
                system(("echo '" + to_string(i) + " " + to_string(shaker.time / (double)1000) + "' >> shaker").c_str());
                system(("echo '" + to_string(i) + " " + to_string(directSelection.time / (double)1000) + "' >> directSelection").c_str());
                system(("echo '" + to_string(i) + " " + to_string(quick.time / (double)1000) + "' >> quick").c_str());
                system(("echo '" + to_string(i) + " " + to_string(shell.time / (double)1000) + "' >> shell").c_str());

                clearScreen();
            }
            system("echo 'plot \
            \"bubble\" smooth bezier,\
            \"shaker\" smooth bezier,\
             \"directSelection\" smooth bezier,\
             \"quick\" smooth bezier,\
             \"shell\" smooth bezier' | gnuplot --persist");
            system("rm bubble");
            system("rm shaker");
            system("rm directSelection");
            system("rm quick");
            system("rm shell");
            break;
        case 4:
            return 0;
        };
    }
    return 0;
}