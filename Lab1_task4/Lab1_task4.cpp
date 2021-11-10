#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#define NOMINMAX
#include <windows.h>
using namespace std;
#include "../genericFunctions.h"

double calculate(double z, double y, double x) {
    double firstHalf = abs(cos(x) - cos(y));
    double secondHalf = (1 + 2 * pow(sin(y), 2));
    double zSquared = z * z;
    return pow(firstHalf, secondHalf) * (1 + z + zSquared / 2.0 + zSquared * z / 3.0 + pow(zSquared, 2) / 4.0);
}

int main()
{
    SetConsoleOutputCP(65001);
    while (true) {
        cout << "\n\tОтвет: " << calculate(inputData("Пожалуйста, введите z = "), inputData("Пожалуйста, введите y = "), inputData("Пожалуйста, введите x = ")) << ", хорошего дня\n" << endl;
        if (!continueOrExit()) {
            break;
        }
    }
    return 0;
}