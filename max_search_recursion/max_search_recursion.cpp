#include "../genericFunctions.h"
#include <iostream>
using namespace std;

void findMaxElemen(double** arr, int size) {
    double maxElement = (*arr)[0];
    for (int i = 1; i < size; i++) {
        if ((*arr)[i] > maxElement) {
            maxElement = (*arr)[i];
        }
    }
    coutWithColor(3, "\nСамое большое число: " + to_string(maxElement) + "\n");
}

double findMaxElemen_recursion(double* arr, int size, int index) {
    if (index < size - 2) {
        return max(arr[index], findMaxElemen_recursion(arr, size, index + 1));
    }
    return arr[size - 1];
}

double findMaxElemen_recursion_concise(double* arr, int index) {
    if (index > 0) {
        return max(arr[index], findMaxElemen_recursion_concise(arr, index - 1));
    }
    return arr[0];
}

int main()
{
    double* dynamic_array;

    int array_size = inputData("Введите размер массива: ");

    dynamic_array = (double*)malloc(array_size * 8 + 1);

    for (int i = 0; i < array_size; i++) {
        dynamic_array[i] = rand() % 10 - 4;
        cout << dynamic_array[i] << " ";
    }

    findMaxElemen(&dynamic_array, array_size);
    coutWithColor(3, "\nСамое большое число (рекурсия): " + to_string(findMaxElemen_recursion(dynamic_array, array_size, 0)) + "\n");
    coutWithColor(3, "\nСамое большое число (рекурсия 2): " + to_string(findMaxElemen_recursion_concise(dynamic_array, array_size - 1)) + "\n");
    
}