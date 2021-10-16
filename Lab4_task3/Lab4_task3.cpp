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

typedef double (*FunctionPointer)(double);

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

string doubleToString(long double value) {
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

string addSpaces(string input, int targetLength) {
    int spaces = targetLength - input.length();
    for (int i = 0; i < spaces; i++) {
        input.append(" ");
    }
    return input;
}

bool* displaySelection(string* options, int optionCount) {

    bool* selectedFunctions = new bool[optionCount];
    for (int i = 0; i < optionCount; i++) {
        selectedFunctions[i] = false;
    }

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
                if (selectedFunctions[i]) {
                    setConsoleColor(counter == i ? 10 : 2);
                } else {
                    setConsoleColor(counter == i ? 12 : 7);
                }
                
                cout << options[i];
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
        }
        if (key == '\r') {
            return selectedFunctions;
        }
        if (key == 8) {
            selectedFunctions[counter] = !selectedFunctions[counter];   
        }
    }
}

double sinXsinX(double x) {
    return sin(x) * sin(x);
}

double cbrtX(double x) {
    return sin(x) * sin(x);
}

double sinXcosX(double x) {
    return sin(x) * cos(x);
}

double expX(double x) {
    return 2 * exp(x);
}

void printTable(FunctionPointer func, double from, double to, double step) {
    int maxX = doubleToString(to).length() + 4;
    for (long double i = from; i * (step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
        cout << "x = " << addSpaces(doubleToString(i), maxX) << "Y(x) = " << func(i) << endl;
    }
}

int main()
{
    SetConsoleOutputCP(65001);

    while (true) {
        long double from = inputData("Введите начальный x: ");
        long double to = inputData("Введите конечный x: ");
        long double step = inputData("Введите шаг: ");
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
       
        coutWithColor(14, "\nВыберите функции (backspace - выбрать/отменить), (enter - подтвердить)\n");

        string functions[4] = { "1. sin(x)*cos(x)", "2. 2*exp(x)", "3. sin(x)*sin(x)", "4. cbrt(x)" };
        FunctionPointer pArray[4] = { sinXcosX, expX, sinXsinX, cbrtX };

        bool* selectedFunctions = displaySelection(functions, 4);

        int selectedFunctionsCount = 0;
        for (int i = 0; i < 4; i++) {
            selectedFunctionsCount += selectedFunctions[i];
        }

        if (selectedFunctionsCount == 0) {
            coutWithColor(8, "\nВы не выбрали ни одной функции(");
        }

        cout << endl;
        for (int i = 0; i < 4; i++) {
            if (selectedFunctions[i]) {
                coutWithColor(10, "\nФункция: "+functions[i]+"\n");
                printTable(pArray[i], from, to, step);
            }
        }

        delete selectedFunctions;

        cout << "\nПродолжить?" << endl;
        string input;
        cin >> input;
        if (!(input == "yes" || input == "y" || input == "1")) {
            break;
        }
    }
}
