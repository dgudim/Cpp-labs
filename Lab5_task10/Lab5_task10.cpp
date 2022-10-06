#include "../genericFunctions.h"
using namespace std;

int *dynamic_array;
int dynamic_array_size;

void fillArray(bool manual) {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (manual) {
            dynamic_array[i] = (int)inputDouble("");
        }
        else {
            dynamic_array[i] = rand() % 10 - 4;
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

void initializeArray (double size_double) {
    int size = (int)size_double;
    if (size_double == 1) {
        cout << "Размер равен 1, мы так ничего не вычислим, надо хотя-бы 3" << endl;
        size = 3;
    }else if (size_double <= 0) {
        cout << "Размер отрицательный или равен нулю, использую размер 10" << endl;
        size = 10;
    }else if (size_double > 1000000) {
        cout << "Размер массива слишком большой, использую размер 1000000" << endl;
        size = 1000000;
    }
    dynamic_array = (int*)malloc(size * 4 + 1);
    dynamic_array_size = size;
    coutWithColor(colors::LIGHT_YELLOW, "\nКак бы вы хотели проинициализировать массив?\n");
    switch (displaySelection({ "1. Случайными числами", "2. Вручную" })) {
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

int findMinElementIndex(bool absolute) {
    int minIndex = 0;
    int minElement = dynamic_array[0];
    for (int i = 1; i < dynamic_array_size; i++) {
        int currElem = absolute ? abs(dynamic_array[i]) : dynamic_array[i];
        if (currElem < minElement) {
            minElement = currElem;
            minIndex = i;
        }
    }
    coutWithColor(colors::LIGHTER_BLUE, "Самое маленькое " + string(absolute ? "по модулю " : "") + "число: " + to_string(minElement) + "(индекс " + to_string(minIndex) + ")\n");
    return minIndex;
}

int findMaxElementIndex() {
    int maxIndex = 0;
    int maxElement = dynamic_array[0];
    for (int i = 1; i < dynamic_array_size; i++) {
        if (dynamic_array[i] > maxElement) {
            maxElement = dynamic_array[i];
            maxIndex = i;
        }
    }
    coutWithColor(colors::LIGHTER_BLUE, "Самое большое число: " + to_string(maxElement) + "(индекс " + to_string(maxIndex) + ")\n");
    return maxIndex;
}

int findLastZeroElementIndex() {
    for (int i = dynamic_array_size - 1; i >= 0; i--) {
        if (dynamic_array[i] == 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс последнего ну л евого элемента: "+to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findFirstZeroElementIndex() {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] == 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс первого нулевого элемента: " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findSecondZeroElementIndex() {
    bool firstZeroFound = false;
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] == 0) {
            if (firstZeroFound) {
                coutWithColor(colors::LIGHTER_BLUE, "Индекс второго нулевого элемента: " + to_string(i) + "\n");
                return i;
            }
            firstZeroFound = true;
        }
    }
    return -2;
}

int findLastPositiveElementIndex() {
    for (int i = dynamic_array_size - 1; i >= 0; i--) {
        if (dynamic_array[i] > 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс последнего полож ительного элемента("+ to_string(dynamic_array[i]) + "): " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findFirstPositiveElementIndex() {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] > 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс первого положительного элемента(" + to_string(dynamic_array[i]) + "): " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findFirstNegativeElementIndex() {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] < 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс первого отрицательного элемент(" + to_string(dynamic_array[i]) + "): " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findLastNegativeElementIndex() {
    for (int i = dynamic_array_size - 1; i >= 0; i--) {
        if (dynamic_array[i] < 0) {
            coutWithColor(colors::LIGHTER_BLUE, "Индекс последнего отрицательного элемента(" + to_string(dynamic_array[i]) + "): " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findSecondNegativeElementIndex() {
    bool secondZeroFound = false;
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] < 0) {
            if (secondZeroFound) {
                coutWithColor(colors::LIGHTER_BLUE, "Индекс второго отрицательного элемента(" + to_string(dynamic_array[i]) + "): " + to_string(i) + "\n");
                return i;
            }
            secondZeroFound = true;
        }
    }
    return -2;
}

void checkIndexes(int& index1, int& index2) {
    if (index1 < -1 || index2 < -1) {
        throw "Один из индексов отрицательный(нет числа, подходящего под критерии в задании)\n";
    }
    if (index1 == index2) {
        throw "Нет элементов между заданными индексами, индексы сопадают\n";
    }
    if (abs(index2 - index1) == 1) {
        throw "Нет элементов между заданными индексами, индексы рядом\n";
    }
    if (index1 > index2) {
        swap(index1, index2);
    }
}

double caclulateSumBetweenTwoIndexes(int index1, int index2, bool absolute) {
    checkIndexes(index1, index2);
    double sum = 0;
    for (int i = index1 + 1; i < index2; i++) {
        sum += absolute ? abs(dynamic_array[i]) : dynamic_array[i];
    }
    return sum;
}

double caclulateProductBetweenTwoIndexes(int index1, int index2) {
    checkIndexes(index1, index2);
    double product = 1;
    for (int i = index1 + 1; i < index2; i++) {
        product *= dynamic_array[i];
    }
    return product;
}

void printResult(double result) {
    coutWithColor(colors::LIGHT_GREEN, "Результат: " + doubleToString(result) + "\n");
}

int main()
{
    srand((size_t)time(NULL));

    while (true) {
        initializeArray(inputDouble("Введите размер массива: "));
        coutWithColor(colors::LIGHT_YELLOW, "\nВыберите что вычислить\n");
        try
        {
            switch (displaySelection({ 
                " 1. Произведение элементов массива, расположенных между максимальным и минимальным элементами",
                " 2. Сумму элементов массива, расположенных между первым и последним нулевыми элементами",
                " 3. Сумму элементов массива, расположенных до последнего положительного элемента",
                " 4. Сумму элементов массива, расположенных между первым и последним положительными элементами",
                " 5. Произведение элементов массива, расположенных между первым и вторым нулевыми элементами",
                " 6. Сумму элементов массива, расположенных между первым и вторым отрицательными элементами",
                " 7. Сумму элементов массива, расположенных до минимального элемента",
                " 8. Сумму модулей элементов массива, расположенных после последнего отрицательного элемента",
                " 9. Сумму элементов массива, расположенных после последнего элемента, равного нулю",
                "10. Сумму модулей элементов массива, расположенных после минимального по модулю элемента",
                "11. Сумму элементов массива, расположенных после минимального элемента",
                "12. Сумму элементов массива, расположенных после первого положительного элемента",
                "13. Сумму модулей элементов массива, расположенных после первого отрицательного элемента",
                "14. Сумму модулей элементов массива, расположенных после первого элемента, равного нулю",
                "15. Сумму положительных элементов массива, расположенных до максимального элемента",
                "16. Произведение элементов массива, расположенных между первым и последним отрицательными элементами" })) {
            case 1:
                printResult(caclulateProductBetweenTwoIndexes(findMaxElementIndex(), findMinElementIndex(false)));
                break;
            case 2:
                printResult(caclulateSumBetweenTwoIndexes(findFirstZeroElementIndex(), findLastZeroElementIndex(), false));
                break;
            case 3:
                printResult(caclulateSumBetweenTwoIndexes(-1, findLastPositiveElementIndex(), false));
                break;
            case 4:
                printResult(caclulateSumBetweenTwoIndexes(findFirstPositiveElementIndex(), findLastPositiveElementIndex(), false));
                break;
            case 5:
                printResult(caclulateProductBetweenTwoIndexes(findFirstZeroElementIndex(), findSecondZeroElementIndex()));
                break;
            case 6:
                printResult(caclulateSumBetweenTwoIndexes(findFirstNegativeElementIndex(), findSecondNegativeElementIndex(), false));
                break;
            case 7:
                printResult(caclulateSumBetweenTwoIndexes(-1, findMinElementIndex(false), false));
                break;
            case 8:
                printResult(caclulateSumBetweenTwoIndexes(findLastNegativeElementIndex(), dynamic_array_size, true));
                break;
            case 9:
                printResult(caclulateSumBetweenTwoIndexes(findLastZeroElementIndex(), dynamic_array_size, false));
                break;
            case 10:
                printResult(caclulateSumBetweenTwoIndexes(findMinElementIndex(true), dynamic_array_size, true));
                break;
            case 11:
                printResult(caclulateSumBetweenTwoIndexes(findMinElementIndex(false), dynamic_array_size, false));
                break;
            case 12:
                printResult(caclulateSumBetweenTwoIndexes(findFirstPositiveElementIndex(), dynamic_array_size, false));
                break;
            case 13:
                printResult(caclulateSumBetweenTwoIndexes(findFirstNegativeElementIndex(), dynamic_array_size, true));
                break;
            case 14:
                printResult(caclulateSumBetweenTwoIndexes(findFirstZeroElementIndex(), dynamic_array_size, true));
                break;
                case 15:{
                    int index1 = -1;
                    int index2 = findMaxElementIndex();
                    checkIndexes(index1, index2);
                    double sum = 0;
                    for (int i = index1 + 1; i < index2; i++) {
                        if (dynamic_array[i] > 0) {
                            sum += dynamic_array[i];
                        }
                    }
                    printResult(sum);
                }
                break;
            case 16:
                printResult(caclulateProductBetweenTwoIndexes(findFirstNegativeElementIndex(), findLastNegativeElementIndex()));
                break;
            }
        }
        catch (const char* message)
        {
            coutWithColor(colors::RED, message);
        }

        free(dynamic_array);
        continueOrExit();
    }
    return 1;
}
