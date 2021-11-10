#include <iostream>
#include <math.h>
#include <string>
#include <conio.h>
#include <sstream>
#include <iomanip>
#define NOMINMAX
#include <windows.h>
using namespace std;

#pragma execution_character_set( "utf-8" )

long double inputData(string message) {
    cout << message << flush;
    long double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while (cin.get() != '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

string doubleToString(long double value){
    ostringstream out;
    out.precision(9);
    out << std::fixed << value;
    string strOut = out.str();
    char currChar = strOut[strOut.length() - 1];
    while ((currChar == '0' || currChar == '.') && strOut.length() > 1) {
        strOut.erase(strOut.length() - 1, 1);
        if (currChar == '.') {
            break;
        }
        currChar = strOut[strOut.length() - 1];
    }
    return strOut;
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

    for (int i = 0; i < optionCount; i++) {
        setConsoleCursorPosition(0, offset + i);
        cout << options[i];
    }

    while (true) {
        if (key == 72) {
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        }
        if (key == 80) {
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++) {
            if (abs(counter - i) <= 1 || i == 0 || i == optionCount - 1) {
                setConsoleCursorPosition(0, offset + i);
                setConsoleColor(counter == i ? 12 : 7);
                cout << options[i];
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
        }
        if (key == '\r') {
            coutWithColor(8, "\nВы выбрали: " + options[counter] + "\n");
            return counter + 1;
        }
    }
}

string addSpaces(string input, int targetLength) {
    int spaces = targetLength - input.length();
    for (int i = 0; i < spaces; i++) {
        input.append(" ");
    }
    return input;
}

long double calculateSum(long double x, int n) {
    if (n == 0) {
        return 1;
    }
    long double sum = 1 - x * x * 1.5;
    if (n == 1) {
        return sum;
    }
    long double curr_bottom = 2;
    long curr_fact_index = 2;
    long double curr_x = x;
    for (int i = 2; i <= n; i++) {
        curr_bottom *= (curr_fact_index + 1) * (curr_fact_index + 2);
        curr_x *= x;
        sum += ((i % 2 == 0) ? 1 : -1 ) * (2 * i * i + 1) * curr_x * curr_x / curr_bottom;
    }
    return sum;
}

int main() {

    SetConsoleOutputCP(65001);

    while (true) {
        long double from = inputData("Введите начальный x: ");
        long double to = inputData("Введите конечный x: ");
        long double step = inputData("Введите шаг: ");
        int n = (int)inputData("Введите n для суммы: ");
        if (from > to && step > 0) {
            cout << "Начальный индекс не может быть меньше конечного, меняю местами" << endl;
            swap(from, to);
        }
        if (step == 0) {
            cout << "Шаг равен 0, вычисление никогда не закончится, теперь шаг = 0.1" << endl;
            step = 0.1;
        }
        if (from == to) {
            cout << "Начальный индекс равен конечному, устанавливаю конечный индекс на 1 больше начального" << endl;
            to++;
        }
        if (abs(to - from) < abs(step)) {
            cout << "Шаг больше, чем промежуток, будет взят только начальный x" << endl;
        }
        if (n < 0) {
            cout << "Количество членов в сумме не может быть меньше 0, использую дефолтное количество : 3 члена" << endl;
            n = 3;
        }
        if ((abs(to) > 70 || abs(from) > 70) && n >= 50) {
            coutWithColor(4, "Начальный или конечный x больше 70 по модулю при n >= 50, возможно переполнение\n");
        }
        if (n > 70) {
            coutWithColor(4, "Количество членов в сумме больше 70, возможно переполнение\n");
        }

        long double current_sum, current_function;
        int maxFuncLen = 25;
        int maxSumLen = 25;
        for (long double i = from; i *(step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
            current_sum = calculateSum(i, n);
            current_function = (1 - i * i / 2) * cos(i) - i / 2 * sin(i);
            maxSumLen = max((int)doubleToString(current_sum).length(), maxSumLen);
            maxFuncLen = max((int)doubleToString(current_function).length(), maxFuncLen);
            cout << "x = " << addSpaces(doubleToString(i), max(doubleToString(from).length(), doubleToString(to).length()) + 3) << "Y(x) = " << addSpaces(doubleToString(current_function), maxFuncLen) << " S(x) = " << addSpaces(doubleToString(current_sum), maxSumLen) << " |S(x)-Y(x)| = " << doubleToString(abs(current_sum - current_function)) << endl;
        }
        cout << "Продолжить?" << endl;
        string input;
        cin >> input;
        if (!(input == "yes" || input == "y" || input == "1")) {
            break;
        }
    }
    return 1;
}