#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "../genericFunctions.h"

using namespace std;

double resolution = 25;
double resolution_sqr = resolution * resolution;

enum class AnimationType {
    LINES,
    FLOW,
    GRID,
    CIRCLE
};

void renderFrame(double phase, AnimationType type);

int main() {
    double k = 0, delta;
    AnimationType animationType;
    switch (displaySelection(new string[4]{
            "1.Линии",
            "2.Поток",
            "3.Сетка",
            "4.Круг"}, 4)){
    case 1:
        delta = 0.001;
        animationType = AnimationType::LINES;
        break;
    case 2:
        delta = 0.05;
        animationType = AnimationType::FLOW;
        break;
    case 3:
        delta = 0.01;
        animationType = AnimationType::GRID;
        break;
    case 4:
        delta = 0.05;
        animationType = AnimationType::CIRCLE;
        break;
    };

    while (true) {
        renderFrame(k, animationType);
        k += delta;
    }
    return 0;
}

void renderFrame(double phase, AnimationType type) {
    setConsoleCursorPosition(0, 0);
    switch (type) {
    case AnimationType::LINES:
        for (int x = -resolution; x < resolution; x++) {
            for (int y = -resolution; y < resolution; y++) {
                setConsoleColor(mapToColor(x * cos(phase) + y * sin(phase), -resolution * sqrt(2), resolution * sqrt(2)));
                cout << "██";
            }
            cout << "\n";
        }
        break;
    case AnimationType::FLOW:
        for (int x = -resolution; x < resolution; x++) {
            for (int y = -resolution; y < resolution; y++) {
                setConsoleColor(mapToColor(x * y + phase, -resolution_sqr, resolution_sqr));
                cout << "██";
            }
            cout << "\n";
        }
        break;
    case AnimationType::GRID:
        for (int x = -resolution; x < resolution; x++) {
            for (int y = -resolution; y < resolution; y++) {
                setConsoleColor(mapToColor((sin(x) + cos(y)) * 10 + phase, -100, 100));
                cout << "██";
            }
            cout << "\n";
        }
        break;
    case AnimationType::CIRCLE:
        for (int x = -resolution; x < resolution; x++) {
            for (int y = -resolution; y < resolution; y++) {
                setConsoleColor(mapToColor(x * x + y * y + phase, -resolution_sqr * 2, resolution_sqr * 2));
                cout << "██";
            }
            cout << "\n";
        }
        break;
    }
    cout << flush;
}