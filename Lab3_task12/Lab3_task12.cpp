#include "../genericFunctions.h"
#include <math.h>
using namespace std;

double calculateSum(double x, int n) {
    if (n == 0) {
        return 1;
    }
    double sum = 1 - x * x * 1.5;
    if (n == 1) {
        return sum;
    }
    double curr_bottom = 2;
    long curr_fact_index = 2;
    double curr_x = x;
    for (int i = 2; i <= n; i++) {
        curr_bottom *= (curr_fact_index + 1) * (curr_fact_index + 2);
        curr_x *= x;
        sum += ((i % 2 == 0) ? 1 : -1 ) * (2 * i * i + 1) * curr_x * curr_x / curr_bottom;
    }
    return sum;
}

int main() {

    while (true) {
        double from = inputDouble("Введите начальный x: ");
        double to = inputDouble("Введите конечный x: ");
        double step = inputDouble("Введите шаг: ");
        int n = (int)inputDouble("Введите n для суммы: ");
        if (from > to && step > 0) {
            cout << "Начальный индекс не может быть меньше конечного, меняю местами" << endl;
            swap(from, to);
        }
        if (step == 0) {
            cout << "Шаг равен 0, вычисление никогда не закончится, теперь шаг = 0.1" << endl;
            step = 0.1;
        }
        if (from == to) {
            cout << "Начальный индекс равен конечному, устанавливаю конечный индекс на 1 больше начального" << endl;
            to++;
        }
        if (abs(to - from) < abs(step)) {
            cout << "Шаг больше, чем промежуток, будет взят только начальный x" << endl;
        }
        if (n < 0) {
            cout << "Количество членов в сумме не может быть меньше 0, использую дефолтное количество : 3 члена" << endl;
            n = 3;
        }
        if ((abs(to) > 70 || abs(from) > 70) && n >= 50) {
            coutWithColor(colors::RED, "Начальный или конечный x больше 70 по модулю при n >= 50, возможно переполнение\n");
        }
        if (n > 150) {
            coutWithColor(colors::RED, "Количество членов в сумме больше 150, возможно переполнение\n");
        }

        double current_sum, current_function;
        int maxFuncLen = 10;
        int maxSumLen = 10;
        for (double i = from; i *(step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
            current_sum = calculateSum(i, n);
            current_function = (1 - i * i / 2) * cos(i) - i / 2 * sin(i);
            maxSumLen = max((int)doubleToString(current_sum).length(), maxSumLen);
            maxFuncLen = max((int)doubleToString(current_function).length(), maxFuncLen);
            cout << "x = " << addSpaces(doubleToString(i), max(doubleToString(from).length(), doubleToString(to).length()) + 3) << "Y(x) = " << addSpaces(doubleToString(current_function), maxFuncLen) << " S(x) = " << addSpaces(doubleToString(current_sum), maxSumLen) << " |S(x)-Y(x)| = " << doubleToString(abs(current_sum - current_function)) << endl;
        }
        continueOrExit();
    }
    return 1;
}
