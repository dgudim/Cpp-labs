
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits>
#include <string>
#include <time.h>
using namespace std;

int *dynamic_array;
int dynamic_array_size;

double inputData(string message) {
    cout << message << flush;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void coutWithColor(int color, string message) {
    setConsoleColor(color);
    cout << message << flush;
    setConsoleColor(7);
}

void setConsoleCursorPosition(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

COORD getConsoleCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

int displaySelection(string* options, int optionCount) {

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int key = 0;

    while (true) {
        if (key == 72){
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        }
        if (key == 80){
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++) {
            setConsoleCursorPosition(0, offset + i);
            setConsoleColor(counter == i ? 12 : 7);
            cout << options[i];
        }
        key = _getch();
        if (key == '\r') {
            coutWithColor(8, "\nВы выбрали: " + options[counter] + "\n");
            return counter + 1;
        }
    }
}

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

void initializeArray (int size_double) {
    int size = (int)size_double;
    if (size_double == 1) {
        cout << "Размер равен 1, мы так ничего не вычислим, надо хотя-бы 3" << endl;
        size = 3;
    }
    if (size_double <= 0) {
        cout << "Размер отрицательный или равен нулю, использую размер 10" << endl;
        size = 10;
    }
    if (size_double > 1000000) {
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

int findMinElementIndex() {
    int minIndex = 0;
    int minElement = dynamic_array[0];
    for (int i = 1; i < dynamic_array_size; i++) {
        if (dynamic_array[i] < minElement) {
            minElement = dynamic_array[i];
            minIndex = i;
        }
    }
    coutWithColor(3, "Самое маленькое число: " + to_string(minElement) + "(индекс " + to_string(minIndex) + ")\n");
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
    coutWithColor(3, "Самое маленькое число: " + to_string(maxElement) + "(индекс " + to_string(maxIndex) + ")\n");
    return maxIndex;
}

int findLastZeroElementIndex() {
    for (int i = dynamic_array_size - 1; i >= 0; i--) {
        if (dynamic_array[i] == 0) {
            coutWithColor(3, "Индекс последнего нулевого элемента: "+to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findFirstZeroElementIndex() {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] == 0) {
            coutWithColor(3, "Индекс первого нулевого элемента: " + to_string(i) + "\n");
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
                coutWithColor(3, "Индекс второго нулевого элемента: " + to_string(i) + "\n");
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
            coutWithColor(3, "Индекс последнего положительного элемента: " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

int findFirstPositiveElementIndex() {
    for (int i = 0; i < dynamic_array_size; i++) {
        if (dynamic_array[i] > 0) {
            coutWithColor(3, "Индекс первого положительного элемента: " + to_string(i) + "\n");
            return i;
        }
    }
    return -2;
}

void checkIndexes(int& index1, int& index2) {
    if (index1 < -1 || index2 < -1) {
        throw "Один из индексов отрицательный";
    }
    if (index1 == index2) {
        throw "Нет элементов между заданными индексами, индексы сопадают";
    }
    if (abs(index2 - index1) == 1) {
        throw "Нет элементов между заданными индексами, индексы рядом";
    }
    if (index1 > index2) {
        swap(index1, index2);
    }
}

double caclulateSumBetweenTwoIndexes(int index1, int index2) {
    checkIndexes(index1, index2);
    double sum = 0;
    for (int i = index1 + 1; i < index2; i++) {
        sum += dynamic_array[i];
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
    setConsoleColor(10);
    cout << "Результат: " << result << endl;
    setConsoleColor(7);
}

int main()
{
    srand((UINT)time(NULL));
    setlocale(0, "RU");

    while (true) {
        initializeArray((int)inputData("Введите размер массива: "));
        coutWithColor(14, "\nВыберите что вычислить\n");
        try
        {
            switch (displaySelection(new string[12]{
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
            "11. Сумму элементов массива, расположенных после минимального элемента" }, 12)) {
            case 1:
                printResult(caclulateProductBetweenTwoIndexes(findMaxElementIndex(), findMinElementIndex()));
                break;
            case 2:
                printResult(caclulateSumBetweenTwoIndexes(findFirstZeroElementIndex(), findLastZeroElementIndex()));
                break;
            case 3:
                printResult(caclulateSumBetweenTwoIndexes(-1, findLastPositiveElementIndex()));
                break;
            case 4:
                printResult(caclulateSumBetweenTwoIndexes(findFirstPositiveElementIndex(), findLastPositiveElementIndex()));
                break;
            case 5:
                printResult(caclulateProductBetweenTwoIndexes(findFirstZeroElementIndex(), findSecondZeroElementIndex()));
                break;
            case 6:

                break;
            case 7:

                break;
            case 8:

                break;
            case 9:

                break;
            case 10:

                break;
            case 11:

                break;
            case 12:

                break;
            }
        }
        catch (const char* message)
        {
            setConsoleColor(4);
            cerr << message << endl;
            setConsoleColor(7);
        }

        free(dynamic_array);

        cout << "Продолжить?" << endl;
        string input;
        cin >> input;
        if (!(input == "yes" || input == "y" || input == "1")) {
            break;
        }
    }
    return 1;
}
