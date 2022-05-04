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
            a = inputData("Введите начало промежутка: ", false);
            b = inputData("Введите конец промежутка: ", false, [a](int input) -> bool {return input > a;}, 
                "Конец должен быть больше начала, повторите ввод: ");
            break;
        case 2:
            divisions = inputData("Введите шаг: ", false, strictPositive,
                "Количество разбиений не может быть меньше или = 0, повторите ввод: ");
            divisions += divisions % 2 != 0;
            
            balanceByEps = false;
            break;
        case 3:
            eps = inputData("Введите новую точность: ", false, [](int input) -> bool {return input < 0.5 && input >= 0.000001;},
                "Точность слишком большая или слишком маленькая (от 0.5 до 0.000001), повторите ввод: ");
                
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
            waitForButtonInput("Нажмите любую кнопку, чтобы вернуться в меню\n");
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