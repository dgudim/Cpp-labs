#include <iostream>
#include <limits>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

double inputDataRaw(string message) {
    cout << message;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

double inputData(string varName) {
    return inputDataRaw("Пожалуйста введите " + varName + ". " + varName + " = ");
}

int main()
{
    setlocale(0, "RU");

    double a;
    double z;
    double x;
    double func_tempVar;

    while (true) {

        a = inputData("a");
        z = inputData("z");

        if (z < 1) {
            x = z * z;
        }
        else {
            x = z + 1;
        }

        cout << "Пожалуйста выберите функцию" << endl;
        cout << "1 - 2x (дефолтная)" << endl;
        cout << "2 - x^2" << endl;
        cout << "3 - x/3" << endl;
        int func = (int)floor(inputDataRaw("Функция: "));

        string selectedFunc = "";

        switch (func) {
        case 1:
            selectedFunc = "2x";
            func_tempVar = 2 * x;
            break;
        case 2:
            selectedFunc = "x^2";
            func_tempVar = x * x;
            break;
        case 3:
            selectedFunc = "x/3";
            func_tempVar = x / 3.0;
            break;
        default:
            cout << "Нет такой функции, использую дефолтную" << endl;
            selectedFunc = "2x";
            func_tempVar = 2 * x;
            break;
        }

        cout << "Вы выбрали " << selectedFunc << endl;

        cout << "Ответ: " << a * log(1 + pow(x, 1.0 / 5.0)) + pow(cos(func_tempVar + 1), 2) << ", хорошего дня" << endl;

        cout << "Продолжить?" << endl;
        string input;
        cin >> input;
        if (!(input == "yes" || input == "y" || input == "1")) {
            break;
        }
    }
}
