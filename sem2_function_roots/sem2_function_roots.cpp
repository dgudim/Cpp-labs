#include <iostream>
#include <math.h>
#include "../genericFunctions.h"
using namespace std;

typedef double (*FunctionPointer)(double);

double findRootOnInterval(double a, double b, double e);

double F(double x) {
    return log(x) - 5 * sin(x) * sin(x);
}

double F2(double x) {
    return 4 * x - 7 * sin(x);
}

double level(double x) {
    return 0;
}

constexpr int field_size = 120;

void printGraph(vector<FunctionPointer> graphs, double from, double to, double step) {

    vector<vector<double>> x_points;
    vector<vector<double>> y_points;

    unsigned int point;

    double minX = from;
    double maxX = to;

    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    for (int g = 0; g < graphs.size(); g++) {
        vector<double> x;
        vector<double> y;
        point = 0;
        for (long double i = from; i * (step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
            x.push_back(i);
            y.push_back(graphs[g](i));
            maxY = max(maxY, y.back());
            minY = min(minY, y.back());
            point++;
        }
        x_points.push_back(x);
        y_points.push_back(y);
    }

    for (int g = 0; g < graphs.size(); g++) {
        for (unsigned int p = 0; p < point; p++) {
            x_points[g][p] -= minX;
            x_points[g][p] /= (maxX - minX);
            x_points[g][p] *= (field_size - 1);

            y_points[g][p] -= minY;
            y_points[g][p] /= (maxY - minY);
            y_points[g][p] *= (field_size - 1);
        }
    }

    int** field = new int* [field_size];
    for (unsigned int i = 0; i < field_size; i++) {
        field[i] = new int[field_size];
    }

    for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
        for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
            field[x_coord][y_coord] = 0;
        }
    }

    for (int g = 0; g < graphs.size(); g++) {
        for (unsigned int p = 0; p < point - 1; p++) {
            unsigned int steps = max(abs((int)x_points[g][p] - (int)x_points[g][p + 1]), abs((int)y_points[g][p] - (int)y_points[g][p + 1])) + 1;
            if (steps > 1) {
                for (unsigned int i = 1; i < steps; i++) {
                    field[(int)lerp(x_points[g][p], x_points[g][p + 1], i / (double)(steps - 1))][(int)lerp(y_points[g][p], y_points[g][p + 1], i / (double)(steps - 1))] += (g + 1);
                }
            } else {
                field[(int)x_points[g][p]][(int)y_points[g][p]] += (g + 1);
            }

        }
    }

    for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
        for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
            coutWithColor(mapToColor(field[x_coord][field_size - y_coord - 1], 0, graphs.size() + 1),
                field[x_coord][field_size - y_coord - 1] > 0 ? "██" : "░░");
        }
        cout << endl;
    }

    for (unsigned int i = 0; i < field_size; ++i) {
        delete[] field[i];
    }
    delete[] field;
}

int main() {
    double a = 2, b = 6, e = 0.0001;
    printGraph({ F2, F, level }, a, b, (b - a) / (double)field_size);
    double res;
    for (double i = a; i < b; i += (b - a) / 5) {
        res = findRootOnInterval(i, i + (b - a) / 5, e);
        if (abs(F(res)) < abs(F(res) - F(res + e))) {
            cout << res << endl;
        }
    }
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