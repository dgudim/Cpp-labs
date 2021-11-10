#include "../genericFunctions.h"
using namespace std;

#pragma execution_character_set( "utf-8" )

typedef double (*FunctionPointer)(double);

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

        bool* selectedFunctions = displayMultiSelection(functions, 4);

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

        delete[] selectedFunctions;
        continueOrExit();
    }
}
