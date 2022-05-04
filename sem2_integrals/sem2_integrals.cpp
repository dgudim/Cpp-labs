#include "../genericFunctions.h"

using namespace std;

double F(double x) {
    return x * x / 6 + cos(x + sin(x)) * x;
}

double simpson_rule(double a, double b, int n);

int main() {

    int a = -7, b = 10;
    int divisions = 20;
    double eps = 0.001;
    int choice;

    bool balanceByEps = false;

    double temp, temp2;
    int divisions_temp;

    while (true) {
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Текущий промежуток: " + to_string(a) + " - " + to_string(b) + "\n");
        coutWithColor(colors::LIGHT_BLUE, "Текущее количество разбиений: " + to_string(divisions) + "\n");
        coutWithColor(colors::LIGHT_BLUE, "Текущая точность: " + to_string(eps) + "\n");
        coutWithColor(colors::LIGHT_GREEN, "Балансирова по " +  string(balanceByEps ? "точности" : "количеству разбиений") + " \n");
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        choice = displaySelection(new string[5]{
                "1.Ввести промежуток",
                "2.Ввести количество разбиений",
                "3.Ввести точность",
                "4.Вычислить",
                "5.Выйти"
            }, 5);
        switch (choice) {
        case 1:
            a = inputData("Введите a: ");
            while (true) {
                b = inputData("Введите b: ");
                if (b > a) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "b должен быть больше a, повторите ввод\n");
            }
            break;
        case 2:
            while (true) {
                divisions = inputData("Введите шаг: ");
                if (divisions > 0) {
                    divisions += divisions % 2 != 0;
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Количество разбиений не может быть меньше или = 0, повторите ввод\n");
            }
            balanceByEps = false;
            break;
        case 3:
            while (true) {
                eps = inputData("Введите точность: ");
                if (eps > 0.00001 && eps < 0.5) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Точность слишком маленькая или слишком большая, повторите ввод\n");
            }
            balanceByEps = true;
            break;
        case 4:
            divisions_temp = divisions * 2;
            temp = simpson_rule(a, b, divisions);
            temp2 = simpson_rule(a, b, divisions_temp);
            if (balanceByEps) {
                while (abs(temp - temp2) >= eps) {
                    divisions_temp *= 2;
                    temp = temp2;
                    temp2 = simpson_rule(a, b, divisions_temp);
                }
                coutWithColor(colors::CYAN, "Результат с точностью " + to_string(eps) + " (" + to_string(divisions_temp) + " разбиений) | " + to_string(temp2) + "\n");
            } else {
                coutWithColor(colors::CYAN, "Результат с количеством разбиений " + to_string(divisions) + " | " + to_string(simpson_rule(a, b, divisions)) + "\n");
            }
            coutWithColor(colors::LIGHTER_BLUE, "Нажмите любую кнопку, чтобы вернуться в меню\n");
            _getch();
            break;
        case 5:
            return 0;
        }
    }

}

double simpson_rule(double a, double b, int n) {
    double h = (b - a) / n;

    double sum_odds = 0;
    for (int i = 1; i < n; i += 2) {
        sum_odds += F(a + i * h);
    }
    double sum_evens = 0;
    for (int i = 2; i < n; i += 2) {
        sum_evens += F(a + i * h);
    }

    return (F(a) + F(b) + 2 * sum_evens + 4 * sum_odds) * h / 3;
}