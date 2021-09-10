#include <iostream>
#include <limits>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

int errCount = 0;

double inputData(string varName) {
    cout << "Пожалуйста введите " << varName << ". " << varName << " = ";
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Пожалуйста используйте числа" << endl;
        errCount++;
        if (errCount == 10) {
            cout << "Ну хватит, пожалуйста" << endl;
        }
    }
    return toReturn;
}

double calculate(double z, double y, double x) {
    double firstHalf = abs(cos(x) - cos(y));
    double secondHalf = (1 + 2 * pow(sin(y), 2));
    double zSquared = z * z;
    return pow(firstHalf, secondHalf) * (1 + z + zSquared / 2 + zSquared * z / 3 + pow(zSquared, 2) / 4);
}

int main()
{
    setlocale(0, "RU");
    while (true) {
        cout << "\n\tВаш результат: " << calculate(inputData("z"), inputData("y"), inputData("x")) << ", хорошего дня" << endl;
        system("pause");
    }
    return 0;
}
