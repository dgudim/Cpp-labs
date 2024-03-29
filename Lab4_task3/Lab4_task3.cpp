#include "Lab4_task3.h"
#include "../genericFunctions.h"

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

int main()
{

    while (true) {
        clearScreen();

        double from;
        double to;
        double step;
        int n;

        coutWithColor(colors::LIGHT_YELLOW, "\nВыберите промежуток, шаг и n для суммы\n");
        switch (displaySelection({
            "1. начальный x = 0, конечный x = 10, шг = 0.1, n = 50", 
            "2. начальный x = 0, конечный x = 1, шаг = 0.05, n = 50",
            "3. начальный x = 10, конечный x = 20, шаг = 0.2, n = 100",
            "4. начальный x = -10, конечный x = 10, шаг = 1, n = 100",
            "5. Ввести вручную"})) {

        case 1:
            from = 0;
            to = 10;
            step = 0.1;
            n = 50;
            break;
        case 2:
            from = 0;
            to = 1;
            step = 0.05;
            n = 50;
            break;
        case 3:
            from = 10;
            to = 20;
            step = 0.2;
            n = 100;
            break;
        case 4:
            from = -10;
            to = 10;
            step = 1;
            n = 100;
            break;
        case 5:
            from = inputDouble("Введите начальный x: ");
            to = inputDouble("Введите конечный x: ");
            step = inputDouble("Введите шаг: ");
            n = inputDouble("Введите n для суммы: ");
            break;
        }


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
            coutWithColor(colors::RED, "Начальный или конечный x больше 70 по модулю при n >= 50, возможно переполнение\n");
        }
        if (n > 150) {
            coutWithColor(colors::RED, "Количество членов в сумме больше 150, возможно переполнение\n");
        }

        coutWithColor(colors::LIGHT_YELLOW, "\nВыберите функции (backspace - выбрать/отменить), (enter - подтвердить)\n");

        vector<string> functions = { "1. (exp(x) + exp(-x)) / 2", "2. exp(x * cos(PI / 4)) * cos(x * sin(PI / 4))", "3. 2 * (cos(x) * cos(x) - 1)" };
        vector<FunctionPointer> pArray = { expXexpX, expXcosX, cosXcosX };
        vector<SumFunctionPointer> pArray_sum = { expXexpX_sum, expXcosX_sum, cosXcosX_sum };

        vector<bool> selectedFunctions = displayMultiSelection(functions);

        int selectedFunctionsCount = 0;
        for (int i = 0; i < functions.size(); i++) {
            selectedFunctionsCount += selectedFunctions[i];
        }
        
        if (selectedFunctionsCount == 0) {
            coutWithColor(colors::GRAY, "\nВы не выбрали ни одной функции(\n");
        }

        coutWithColor(colors::LIGHT_YELLOW, "\nВыберите дополнительные опции\n");
        vector<bool> selectedOptions = displayMultiSelection({ "1. Печатать сумму", "2. Печатать |S-Y|", "3. Печатать график" });

        cout << endl;
        for (int i = 0; i < functions.size(); i++) {
            if (selectedFunctions[i]) {
                coutWithColor(colors::LIGHT_GREEN, "\nФункция: " + functions[i] + "\n");
                printTable(pArray[i], pArray_sum[i], from, to, step, n, selectedOptions[0], selectedOptions[1], selectedOptions[2]);
            }
        }

        continueOrExit();
    }
}

double cosXcosX(double x) {
    return 2 * (cos(x) * cos(x) - 1);
}

double cosXcosX_sum(double x, int n) {
    double r, s = 0, y = 2, t, z = 1;
    r = (-1) * (4 * x * x) / 2;
    for (int k = 1; k <= n; k += 1)
    {
        s += r;
        t = (-1) * (4 * x * x) / ((z += 2) * (y += 2));
        r = r * t;
    }
    return s;
}

double expXcosX(double x) {
    return exp(x * cos(M_PI / 4)) * cos(x * sin(M_PI / 4));
}

double expXcosX_sum(double x, int n) {
    double p, s;
    p = s = 1;
    for (int k = 1; k <= n; k++)
    {
        p *= x / k;
        s += cos(k * M_PI / 4) * p;

    }
    return s;
}

double expXexpX(double x) {
    return (exp(x) + exp(-x)) / 2;
}

double expXexpX_sum(double x, int n) {
    double t, y = 1, z = 2, r, s = 1;;
    r = (x * x / 2);

    for (int k = 2; k <= n; k++)
    {
        s += r;
        t = (x * x) / ((y += 2) * (z += 2));
        r = r * t;
    }

    return s;
}

void printTable(FunctionPointer func, SumFunctionPointer func_sum, double from, double to, double step, int n, bool print_sum, bool print_abs, bool print_graph) {
    int maxX = doubleToString(to).length() + 4;
    double y = 0, sum = 0;
    for (long double i = from; i * (step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
        y = func(i);
        if (print_sum || print_abs) {
            sum = func_sum(i, n);
        }
        cout << "x = " << addSpaces(doubleToString(i), maxX) << "Y(x) = " << addSpaces(doubleToString(y), 13);
        if (print_sum) {
            cout << " S(x) = " << addSpaces(doubleToString(sum), 13);
        }
        if (print_abs) {
            cout << " |S(x) - Y(x)| = " << addSpaces(doubleToString(abs(y - sum)), 13);
        }
        cout << endl;
    }
    if (print_graph) {
        printGraph({ func }, from, to, step, 45);
    }
}
