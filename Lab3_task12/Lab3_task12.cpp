#include <iostream>
#include <math.h>
#include <string>
#include <conio.h>
#define NOMINMAX
#include <windows.h>
using namespace std;

typedef long double (*CalcFuncPointer)(double);

long double defaultFunction(double x) {
    return (1 - x * x / 2) * cos(x) - x / 2 * sin(x);
}

long double expFunction(double x) {
    return 2 * exp(x);
}

long double sin2Function(double x) {
    return sin(x) * sin(x);
}

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

long double calculateSum(double x, int n) {
    if (n == 0) {
        return 1;
    }
    long double sum = 1 - x * x * 1.5;
    if (n == 1) {
        return sum;
    }
    long double intermediate_top = 0;
    char curr_sign = -1;
    long double curr_bottom = 2;
    long curr_bottom_fact_index = 2;
    long double curr_x = x;
    for (int i = 2; i <= n; i++) {
        intermediate_top = 2 * i * i + 1;
        for (int f = curr_bottom_fact_index + 1; f <= i * 2; f++) {
            curr_bottom *= f;
        }
        curr_sign *= -1;
        curr_x *= x;
        sum += curr_sign * intermediate_top * curr_x * curr_x / curr_bottom;
    }
    return sum;
}

int main() {

    setlocale(0, "RU");

    while (true) {
        double from = inputData("Введите начальный x: ");
        double to = inputData("Введите конечный x: ");
        double step = inputData("Введите шаг: ");
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
        if (n < 0) {
            cout << "Количество членов в сумме не может быть меньше 0, использую дефолтное количество: 3 члена" << endl;
            n = 3;
        }
        if ((abs(to) > 700 || abs(from) > 700) && n >= 5) {
            coutWithColor(4, "Начальный или конечный x больше 700 по модулю при n >= 5, возможно переполнение\n");
        }
        if (n > 70) {
            coutWithColor(4, "Количество членов в сумме больше 70, возможно переполнение или очень долгое вычисление\n");
        }

        coutWithColor(14, "\nВыберите функцию Y(x)\n");
        CalcFuncPointer func;
        switch (displaySelection(new string[3]{ "1. (1 - x^2 / 2) * cos(x) - x / 2*sin(x)", "2. 2*exp(x)", "3. sin(x)^2" }, 3)) {
            case(1):
            default:
                func = defaultFunction;
                break;
            case(2):
                func = expFunction;
                break;
            case(3):
                func = sin2Function;
                break;
        }

        long double current_sum, current_function;
        int maxFuncLen = 15;
        int maxSumLen = 15;
        for (double i = from; i *(step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
            current_sum = calculateSum(i, n);
            current_function = func(i);
            maxSumLen = max((int)to_string(current_sum).length(), maxSumLen);
            maxFuncLen = max((int)to_string(current_function).length(), maxFuncLen);
            cout << "x = " << addSpaces(to_string(i), max(to_string(from).length(), to_string(to).length()) + 3) << "Y(x) = " << addSpaces(to_string(current_function), maxFuncLen) << " S(x) = " << addSpaces(to_string(current_sum), maxSumLen) << " |S(x)-Y(x)| = " << to_string(abs(current_sum - current_function)) << endl;
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