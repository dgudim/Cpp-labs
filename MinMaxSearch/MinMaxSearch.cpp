#include "../genericFunctions.h"
using namespace std;

int* dynamic_array;
int dynamic_array_size;

void fillArray(bool manual) {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (manual) {
            dynamic_array[i] = (int)inputData("");
        }
        else {
            dynamic_array[i] = rand() % 100 - 49;
            cout << dynamic_array[i] << " ";
        }
    }
    if (manual) {
        cout << "Ваш массив(проверьте переполнения): ";
        for (int i = 0; i < dynamic_array_size; i++) {
            cout << dynamic_array[i] << " ";
        }
    }
    cout << endl;
}

void initializeArray(double size_double) {
    int size = (int)size_double;
    if (size_double == 1) {
        cout << "Размер равен 1, мы так ничего не вычислим, надо хотя-бы 3" << endl;
        size = 3;
    }
    else if (size_double <= 0) {
        cout << "Размер отрицательный или равен нулю, использую размер 10" << endl;
        size = 10;
    }
    else if (size_double > 1000000) {
        cout << "Размер массива слишком большой, использую размер 1000000" << endl;
        size = 1000000;
    }
    dynamic_array = (int*)malloc(size * 4 + 1);
    dynamic_array_size = size;
    coutWithColor(14, "\nКак бы вы хотели проинициализировать массив?\n");
    switch (displaySelection(new string[2]{ "1. Случайными числами", "2. Вручную" }, 2)) {
    case 1:
        cout << "Заполняю ваш массив случайными числами..." << endl;
        fillArray(false);
        break;
    case 2:
        cout << "                           Массив = ";
        fillArray(true);
        break;
    }
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    SetConsoleOutputCP(65001);
    while (true) {
        initializeArray(inputData("Введите размер массива: "));
        quickSort(dynamic_array, 0, dynamic_array_size - 1);

        cout << "Сортированный массив: " << endl;
        for (int i = 0; i < dynamic_array_size; i++) {
            cout << dynamic_array[i] << " " << flush;
        }
        cout << endl;

        int differentNumbers = 1;
        for (int i = 0; i < dynamic_array_size - 1; i++) {
            if (dynamic_array[i] != dynamic_array[i + 1]) {
                differentNumbers++;
            }
        }
        int* numbers_counter = (int*)malloc(differentNumbers * 4 + 1);
        int* numbers_unique = (int*)malloc(differentNumbers * 4 + 1);
        int* numbers_precedence = (int*)malloc(differentNumbers * 4 + 1);

        int currentIndex = 0;
        int currentNumberCount = 0;
        for (int i = 1; i < dynamic_array_size + 1; i++) {
            currentNumberCount++;
            if ((dynamic_array[i] != dynamic_array[i - 1]) || i == dynamic_array_size) {
                numbers_unique[currentIndex] = dynamic_array[i - 1];
                numbers_counter[currentIndex] = currentNumberCount;
                numbers_precedence[currentIndex] = (differentNumbers - currentIndex) * currentNumberCount;
                currentNumberCount = 0;
                currentIndex++;
            }
        }

        cout << "\nУникальные числа: " << endl;
        for (int i = 0; i < differentNumbers; i++) {
            cout << numbers_unique[i] << " " << flush;
        }

        cout << "\nИх количества: " << endl;
        for (int i = 0; i < differentNumbers; i++) {
            cout << numbers_counter[i] << " " << flush;
        }

        cout << "\nИх преимущества: " << endl;
        for (int i = 0; i < differentNumbers; i++) {
            cout << numbers_precedence[i] << " " << flush;
        }

        int maxPrecedence = 1;
        int maxPrecedenceIndex = 0;
        for (int i = 0; i < differentNumbers; i++) {
            if (numbers_precedence[i] > maxPrecedence) {
                maxPrecedenceIndex = i;
                maxPrecedence = numbers_precedence[i];
            }
        }

        cout << "\nМинимальное число, встречающееся максимальное количество раз: " << numbers_unique[maxPrecedenceIndex] << endl;

        free(dynamic_array);
        free(numbers_counter);
        free(numbers_unique);
        free(numbers_precedence);

        continueOrExit();
    }
    return 1;
}
