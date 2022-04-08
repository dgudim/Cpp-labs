#include <iostream>
#include <math.h>
#include "../genericFunctions.h"
using namespace std;

double findRootOnInterval(double a, double b, double e, int& iterations);

vector<double> root_convergence;
double current_root_level;

double F(double x) {
    return log(x) - 5 * sin(x) * sin(x);
}

double level(double x) {
    return 0;
}

double convergence(double x) {
    return root_convergence.at(x);
}

double root_level(double x) {
    return current_root_level;
}

constexpr int field_size = 45;

int main() {
    double a = 2, b = 6, e = 0.001;
    double res;
    int iter;
    while(true){
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Текущая точность: " + to_string(e) + "\n");
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        switch (displaySelection(new string[4]{
            "1.Ввести e (точность)",
            "2.Вычислить с помощью метода деления отрезка пополам",
            "3.Вычислить с помощью метода парабол",
            "4.Выйти"}, 4))
        {
        case 1:
            while (true) {
                e = inputData("Введите новую точность: ");
                if (e < 0.5 && e >= 0.000001) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Точность слишком большая или слишком маленькая, повторите ввод\n");
            }
            break;
        case 2:
            coutWithColor(colors::YELLOW, "График:\n");

            printGraph({ F, level }, a, b, (b - a) / (double)field_size, field_size);

            for (double i = a; i < b; i += (b - a) / 5) {
                res = findRootsOnInterval_interval_division(i, i + (b - a) / 5, e, iter);
                if (abs(F(res)) < abs(F(res) - F(res + e))) {
                    coutWithColor(colors::LIGHT_GREEN, "Корень " + to_string(res) + " | f(x)=" + to_string(F(res)) + " | " + to_string(iter) + " итераций\n");
                    coutWithColor(colors::YELLOW, "Схождение:\n");
                    printGraph({ convergence, root_level }, 0, root_convergence.size() - 1, 1, field_size);
                }
            }
            coutWithColor(colors::LIGHTER_BLUE, "Нажмите любую кнопку, чтобы вернуться в меню\n");
            _getch();
            break;
        case 4:
            return 0;
        }
    }
    return 0;
}

double findRootsOnInterval_interval_division(double a, double b, double e, int& iterations) {
    root_convergence.clear();
    double res, x0 = a, x1 = b, x2;
    double y0, y1, y2;
    y0 = F(x0);
    y1 = F(x1);
    iterations = 0;
    while (true) {
        iterations++;
        x2 = (x0 + x1) / 2;
        y2 = F(x2);
        if (y0 * y2 > 0) {
            x0 = x2;
            y0 = y2;
        } else {
            x1 = x2;
            root_convergence.push_back(x1);
            y1 = y2;
        }
        if (x1 - x0 <= e) {
            res = (x0 + x1) / 2;
            current_root_level = res;
            return res;
        }
    }
}