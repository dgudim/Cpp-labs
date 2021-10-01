
#include <iostream>
#include <math.h>
using namespace std;

double inputData(string message) {
    cout << message << flush;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

double calculateFunction(double x) {
    return (1 - x * x / 2) * cos(x) - x / 2 * sin(x);
}

double calculateSum(double x, int n) {
    double sum = 0;
    double intermediate_top = 0;
    char curr_sign = -1;
    double curr_top = 0;
    double curr_bottom = 2;
    double curr_x = x;
    for (int i = 2; i <= n; i++) {
        intermediate_top = 2 * i * i + 1;
        curr_bottom *= 2 * i;
        curr_sign *= -1;
        curr_x *= x;
        sum += curr_sign * intermediate_top * curr_x * curr_x / curr_bottom;
    }
    return sum;
}

int main() {

    setlocale(0, "RU");

    double from = inputData("Введите начальный x: ");
    double to = inputData("Введите конечный x: ");
    double step = inputData("Введите шаг: ");
    int n = (int)inputData("Введите n для суммы: ");
    for (double i = from; i <= to; i += step) {
        cout << "x = " << i << " | Y(x): " << calculateFunction(i) << " | Сумма: " << calculateSum(i, n) << endl;
    }
    return 1;
}