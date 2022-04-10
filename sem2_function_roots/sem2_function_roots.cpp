#include <iostream>
#include <math.h>
#include <algorithm>
#include "../genericFunctions.h"

using namespace std;

double findRootsOnInterval_interval_division(double a, double b, double e, int& iterations);
double findRootsOnInterval_interval_porabolic(double x0, double h, double e, int& iterations);

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
    int choice;
    double step = (b - a) / 5;
    vector<int> results;
    while (true) {
        results.clear();
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Текущая точность: " + to_string(e) + "\n");
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        choice = displaySelection(new string[4]{
            "1.Ввести e (точность)",
            "2.Вычислить с помощью метода деления отрезка пополам",
            "3.Вычислить с помощью метода парабол",
            "4.Выйти"}, 4);
        switch (choice) {
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
        case 3:
            coutWithColor(colors::YELLOW, "График:\n");

            printGraph({ F, level }, a, b, (b - a) / (double)field_size, field_size);

            for (double i = a; i < b; i += step) {
                if (choice == 2) {
                    res = findRootsOnInterval_interval_division(i, i + step, e, iter);
                } else {
                    res = findRootsOnInterval_interval_porabolic(i, step / 3, e, iter);
                }

                if (abs(F(res)) < abs(F(res) - F(res + e)) && res >= a && res <= b && 
                    find(results.begin(), results.end(), (int)(res / e / 10)) == results.end()) {
                    results.push_back((int)(res / e / 10));
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

double findRootsOnInterval_interval_porabolic(double x0, double h, double e, int& iterations) {
    root_convergence.clear();
    double x1 = x0 - h;
    double x2 = x0;
    double x3 = x0 + h;
    double y1 = F(x1);
    double y2 = F(x2);
    double y3 = F(x3);
    double z1, z2, r, d, p, q, D, zm;
    iterations = 0;
    while (true) {
        iterations++;
        z1 = x1 - x3;
        z2 = x2 - x3;
        r = y3;
        d = z1 * z2 * (z1 - z2);
        p = ((y1 - y3) * z2 - (y2 - y3) * z1) / d;
        q = -((y1 - y3) * z2 * z2 - (y2 - y3) * z1 * z1) / d;
        D = sqrt(q * q - 4 * p * r);
        zm = min(abs((-q + D) / (2 * p)), abs((-q - D) / (2 * p)));
        x1 = x2;
        x2 = x3;
        y1 = y2;
        y2 = y3;
        x3 = x3 + zm;
        y3 = F(x3);
        root_convergence.push_back(x3);
        if (zm < e || iterations > 100) {
            current_root_level = x3;
            return x3;
        }
    }
}