#include "Lab4_task3.h"
#include "../genericFunctions.h"

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

#pragma execution_character_set( "utf-8" )

int main()
{
    SetConsoleOutputCP(65001);

    while (true) {
        system("CLS");

        double from;
        double to;
        double step;
        int n;

        coutWithColor(14, "\nВыберите промежуток, шаг и n для суммы\n");
        switch (displaySelection(new string[5]{ 
            "1. начальный x = 0, конечный x = 10, шаг = 0.1, n = 50", 
            "2. начальный x = 0, конечный x = 1, шаг = 0.05, n = 50",
            "3. начальный x = 10, конечный x = 20, шаг = 0.2, n = 100",
            "4. начальный x = -10, конечный x = 10, шаг = 1, n = 100",
            "5. Ввести вручную"}, 5)) {
        
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
            from = inputData("Введите начальный x: ");
            to = inputData("Введите конечный x: ");
            step = inputData("Введите шаг: ");
            n = (int)inputData("Введите n для суммы: ");
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
            coutWithColor(4, "Начальный или конечный x больше 70 по модулю при n >= 50, возможно переполнение\n");
        }
        if (n > 150) {
            coutWithColor(4, "Количество членов в сумме больше 150, возможно переполнение\n");
        }
       
        coutWithColor(14, "\nВыберите функции (backspace - выбрать/отменить), (enter - подтвердить)\n");

        const unsigned int functions_len = 3;

        string functions[functions_len] = { "1. (exp(x) + exp(-x)) / 2", "2. exp(x * cos(PI / 4)) * cos(x * sin(PI / 4))", "3. 2 * (cos(x) * cos(x) - 1)"};
        FunctionPointer pArray[functions_len] = {expXexpX, expXcosX, cosXcosX};
        SumFunctionPointer pArray_sum[functions_len] = {expXexpX_sum, expXcosX_sum, cosXcosX_sum};

        bool* selectedFunctions = displayMultiSelection(functions, functions_len);

        int selectedFunctionsCount = 0;
        for (int i = 0; i < functions_len; i++) {
            selectedFunctionsCount += selectedFunctions[i];
        }

        if (selectedFunctionsCount == 0) {
            coutWithColor(8, "\nВы не выбрали ни одной функции(");
        }

        coutWithColor(14, "\nВыберите дополнительные опции\n");
        bool* selectedOptions = displayMultiSelection(new string[3]{"1. Печатать сумму", "2. Печатать |S-Y|", "3. Печатать график" }, 3);

        cout << endl;
        for (int i = 0; i < functions_len; i++) {
            if (selectedFunctions[i]) {
                coutWithColor(10, "\nФункция: "+functions[i]+"\n");
                printTable(pArray[i], pArray_sum[i], from, to, step, n, selectedOptions[0], selectedOptions[1], selectedOptions[2]);
            }
        }

        delete[] selectedFunctions;
        delete[] selectedOptions;
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

double lerp(double from, double to, double progress) {
    return from * (1 - progress) + to * progress;
}

void printGraph(FunctionPointer func, double from, double to, double step) {
    unsigned int field_size = 45;
    unsigned int points = (int)((to - from) / step + 1);

    double* x = new double[points];
    double* y = new double[points];

    unsigned int point = 0;

    double minX = from;
    double maxX = to;

    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    for (long double i = from; i * (step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
        x[point] = i;
        y[point] = func(i);
        maxY = max(maxY, y[point]);
        minY = min(minY, y[point]);
        point++;
    }

    for (unsigned int p = 0; p < point; p++) {
        x[p] -= minX;
        x[p] /= (maxX - minX);
        x[p] *= (field_size - 1);

        y[p] -= minY;
        y[p] /= (maxY - minY);
        y[p] *= (field_size - 1);
    }

    bool** field = new bool* [field_size];
    for (unsigned int i = 0; i < field_size; i++) {
        field[i] = new bool[field_size];
    }

    for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
        for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
            field[x_coord][y_coord] = false;
        }
    }

    for (unsigned int p = 0; p < point - 1; p++) {
        unsigned int steps = max(abs((int)x[p] - (int)x[p + 1]), abs((int)y[p] - (int)y[p + 1])) + 1;
        if (steps > 1) {
            for (unsigned int i = 0; i < steps; i++) {
                field[(int)lerp(x[p], x[p + 1], i / (double)(steps - 1))][(int)lerp(y[p], y[p + 1], i / (double)(steps - 1))] = true;
            }
        }
        else {
            field[(int)x[p]][(int)y[p]] = true;
        }

    }

    for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
        for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
            cout << (field[x_coord][field_size - y_coord - 1] ? "██" : "░░");
        }
        cout << endl;
    }

    delete[] x;
    delete[] y;
    for (unsigned int i = 0; i < field_size; ++i) {
        delete[] field[i];
    }
    delete[] field;
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
        printGraph(func, from, to, step);
    }
}
