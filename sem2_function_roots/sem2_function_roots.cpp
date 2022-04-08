#include <iostream>
#include <math.h>
#include "../genericFunctions.h"
using namespace std;

double findRootOnInterval(double a, double b, double e);

double F(double x) {
    return log(x) - 5 * sin(x) * sin(x);
}

void printGraph(double from, double to, double step) {
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
        y[point] = F(i);
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
        } else {
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

int main() {
    double a = 2, b = 6, e = 0.0001;
    printGraph(a, b, (b - a) / 45);
    double res = findRootOnInterval(a, b, e);
    _getch();

    return 0;
}

double findRootOnInterval(double a, double b, double e) {
    double res, x0 = a, x1 = b, x2;
    double y0, y1, y2;
    y0 = F(x0);
    y1 = F(x1);
    while (true) {
        x2 = (x0 + x1) / 2;
        y2 = F(x2);
        if (y0 * y2 > 0) {
            x0 = x2;
            y0 = y2;
        } else {
            x1 = x2;
            y1 = y2;
        }
        if (x1 - x0 <= e) {
            res = (x0 + x1) / 2;
            return res;
        }
    }
}