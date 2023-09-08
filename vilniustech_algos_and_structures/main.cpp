#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main() {
    int array[] = {1, 2, 3, -2, 5, 6, 7};

    int size = sizeof(array) / sizeof(array[0]);

    cout << "Size: " << size << "\n";

    int neg_sum = 0;

    int smallest_index = 0;
    int largest_index = 0;
    int smallest = array[0];
    int largest = array[0];

    for (int i = 0; i < size; i++) {
        int num = array[i];
        if (num < 0) {
            neg_sum += num;
        }
        if (num < smallest) {
            smallest = num;
            smallest_index = i;
        }
        if (num > largest) {
            largest = num;
            largest_index = i;
        }
    }

    int smallest_index_orig = smallest_index;
    if (smallest_index > largest_index) {
        smallest_index = largest_index;
        largest_index = smallest_index_orig;
    }

    int product = 1;
    for (int i = smallest_index + 1; i < largest_index; i++) {
        product *= array[i];
    }

    cout << "Sum of negative elements: " << neg_sum << "\n";
    cout << "Product of min and max: " << product << "\n";
    cout << "Max element: " << largest << "\n";

    int sum_up_to_last_positive = 0;
    bool found = false;

    for (int i = size - 1; i >= 0; i--) {
        int num = array[i];
        if (num > 0 || found) {
            found = true;
            sum_up_to_last_positive += num;
        }
    }

    cout << "The sum of the elements up to the last positive element: "
         << sum_up_to_last_positive << "\n";

    int range_upper = 3;
    int range_lower = -3;

    for (int i = 0; i < size; i++) {
        int num = array[i];
        if (num >= range_lower && num <= range_upper) {
            array[i] = 0;
        }
    }

    int *new_array = (int *)malloc((size + 1) * sizeof(int));
    for (int i = 0; i < smallest_index; i++) {
        new_array[i] = array[i];
    }
    new_array[smallest_index] = 999;
    for (int i = smallest_index + 1; i < size; i++) {
        new_array[i + 1] = array[i];
    }

    cout << "Final array: ";
    for (int i = 0; i < size + 1; i++) {
        cout << new_array[i] << " ";
    }

    cout << "\n";

    return 0;
}
