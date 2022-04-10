#include <iostream>
#define _USE_MATH_DEFINES;
#include <math.h>
#include <algorithm>
#include "../genericFunctions.h"

using namespace std;

double resolution = 25;
double resolution_sqr = resolution * resolution;

int main() {
    double k = 0;
    while(true){
        setConsoleCursorPosition(0, 0);

        // for (int x = -resolution; x < resolution; x++) {
        //     for (int y = -resolution; y < resolution; y++) {
        //         setConsoleColor(mapToColor(x * cos(k) + y * sin(k), -resolution * sqrt(2), resolution * sqrt(2)));
        //         cout << "██";
        //     }
        //     cout << "\n";
        // }

        // for (int x = -resolution; x < resolution; x++) {
        //     for (int y = -resolution; y < resolution; y++) {
        //         setConsoleColor(mapToColor(x * y + k, -resolution_sqr, resolution_sqr));
        //         cout << "██";
        //     }
        //     cout << "\n";
        // }

        // for (int x = -resolution; x < resolution; x++) {
        //     for (int y = -resolution; y < resolution; y++) {
        //         setConsoleColor(mapToColor((sin(x) + cos(y)) * 10 + k, -100, 100));
        //         cout << "██";
        //     }
        //     cout << "\n";
        // }

        for (int x = -resolution; x < resolution; x++) {
            for (int y = -resolution; y < resolution; y++) {
                setConsoleColor(mapToColor(x*x + y*y + k, -resolution_sqr * 2, resolution_sqr * 2));
                cout << "██";
            }
            cout << "\n";
        }

        k += 0.01;
        cout << flush;
    }
    return 0;
}