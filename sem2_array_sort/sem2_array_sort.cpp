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

enum FillType {
    Random, Forward, Reverse
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

Metrics benchmarkSort(SortFunction sortFunction, const string& name, const vector<ArrayStruct>& orig, bool print = true) {
    unsigned int iterations = 0, changes = 0;
    auto start = high_resolution_clock::now();
    vector<ArrayStruct> sorted = sortFunction(orig, iterations, changes);
    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
    bool sorted_s = checkIfSorted(sorted);
    if(print) {
        if (sorted_s) {
            coutWithColor(colors::LIGHT_GREEN, "Result for ");
            coutWithColor(colors::CYAN, name);
            coutWithColor(colors::LIGHT_GREEN, " on ");
            coutWithColor(colors::PURPLE, to_string(orig.size()));
            coutWithColor(colors::LIGHT_GREEN, " elements: ");
            coutWithColor(colors::YELLOW, to_string(iterations));
            coutWithColor(colors::LIGHT_GREEN, " iterations, ");
            coutWithColor(colors::YELLOW, to_string(changes));
            coutWithColor(colors::LIGHT_GREEN, " array changes, ");
            coutWithColor(colors::YELLOW, doubleToString(duration.count() / 1000.0));
            coutWithColor(colors::LIGHT_GREEN, "ms\n");
        } else {
            coutWithColor(colors::LIGHT_RED, "Sorting error using " + name + "!\n");
        }
    }
    return { iterations, changes, duration.count() };
}

void refillArray(vector<ArrayStruct>& values, int n, FillType fillType) {
    values.clear();
    for (int i = 0; i < n; i++) {
        values.push_back(ArrayStruct(fillType == FillType::Random ? rand() : (fillType == FillType::Reverse ? n -i : i), ""));
    }
}

int main() {
    int a, b;
    int n = 25;

    srand(time(NULL));

    vector<ArrayStruct> orig;
    vector<int> values_time, values_changes;
    vector<string> titles_time, titles_changes;

    Metrics algo_metrics;

    FillType fillType = FillType::Random;

    vector<SortFunction> sortingFunctions = {
        bubbleSort,
        shakerSort,
        directSelectionSort,
        quickSortWrapper,
        shellSort };
    vector<string> sortingFunctions_displayNames = {
        "bubble sort",
        "shaker sort",
        "selection sort",
        "quick sort",
        "shell sort"};
    vector<string> sortingFunctions_fileNames = {
        "bubble",
        "shaker",
        "directSelection",
        "quick",
        "shell" };

    unsigned int elems = min(min(sortingFunctions.size(), sortingFunctions_displayNames.size()), sortingFunctions_fileNames.size());

    string plot_string, plot_string2;

    while (true) {
        refillArray(orig, n, fillType);
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Current array size: " + to_string(n) + "\n");
        string fillTypeStr = "";
        switch(fillType) {
            case FillType::Random:
                fillTypeStr = "unsorted";
                break;
            case FillType::Forward:
                fillTypeStr = "sorted";
                break;
            case FillType::Reverse:
                fillTypeStr = "backwards-sorted";
                break;
        }
        coutWithColor(colors::LIGHT_GREEN, "Current array contents: " + fillTypeStr + "\n");
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        switch (displaySelection({
                 "1.Enter array size",
                 "2.Set array contents",
                 "3.Sort and see the results",
                 "4.Graph relation of execution time and array changes to the number of elements",
                 "5.Exit"
            })) {
        case 1:
            n = (int)inputDouble("Enter size: ", false, strictPositive,
                "Size should be >= 0, retry: ");
            break;
        case 2:
            fillType = (FillType)(displaySelection({
                 "1.Unsorted",
                 "2.Sorted",
                 "3.Backwards-sorted"
            }) - 1);
            break;
        case 3:

            values_time.clear();
            titles_time.clear();
            values_changes.clear();
            titles_changes.clear();

            for (int i = 0; i < elems; i++) {
                coutWithColor(mapToColor(i, 0, elems), "█");
                algo_metrics = benchmarkSort(sortingFunctions[i], sortingFunctions_displayNames[i], orig);

                values_time.push_back(algo_metrics.time);
                titles_time.push_back(doubleToString(algo_metrics.time / (float)1000) + "ms");

                values_changes.push_back(algo_metrics.changes);
                titles_changes.push_back(to_string(algo_metrics.changes));
            }

            coutWithColor(colors::LIGHT_PURPLE, "\nExecution time\n");
            printBarChart(values_time, titles_time, 64, 8, 2);

            coutWithColor(colors::LIGHT_PURPLE, "Array changes\n");
            printBarChart(values_changes, titles_changes, 64, 8, 2);

            waitForButtonInput("Press any button to return to main menu\n");
            break;
        case 4:

            a = (int)inputDouble("Enter start of range: ", false, strictPositive,
                "Start should be >= 0, retry:: ");
            b = (int)inputDouble("Enter end or range: ", false, [a](int input) -> bool {return input > a;},
                "End should be > start, retry: ");

            clearScreen();

            for (int i = a; i <= b; i ++) {
                refillArray(orig, i, fillType);
                displayLoadingIcon(loadingIconStyle::DOTS_TALL, colors::RED, i / 20.0, 1, 1);
                displayLoadingBar(colors::GRAY, colors::CYAN, 25, (i - a) / (float)(b - a), 3, 1);
                for (int el = 0; el < elems; el++) {
                    algo_metrics = benchmarkSort(sortingFunctions[el], sortingFunctions_displayNames[el], orig, false);
                    system(("echo '" + to_string(i) + " " + to_string(algo_metrics.time / (double)1000) + "' >> " + sortingFunctions_fileNames[el]).c_str());
                    system(("echo '" + to_string(i) + " " + to_string(algo_metrics.changes) + "' >> " + sortingFunctions_fileNames[el] + "_changes").c_str());
                }
            }
            plot_string.clear();
            plot_string2.clear();

            plot_string += "echo 'plot ";
            plot_string2 += "echo 'plot ";
            for (int el = 0; el < elems; el++) {
                if (el > 0) {
                    plot_string += ", ";
                    plot_string2 += ", ";
                }
                plot_string += "\"" + sortingFunctions_fileNames[el] + "\" smooth bezier";
                plot_string2 += "\"" + sortingFunctions_fileNames[el] + "_changes" + "\" smooth bezier";
            }
            plot_string += "' | gnuplot --persist";
            plot_string2 += "' | gnuplot --persist";
            system(plot_string.c_str());
            system(plot_string2.c_str());
            for (int el = 0; el < elems; el++) {
                system(("rm " + sortingFunctions_fileNames[el]).c_str());
                system(("rm " + sortingFunctions_fileNames[el] + "_changes").c_str());
            }
            break;
        case 5:
            return 0;
        };
    }
    return 0;
}
