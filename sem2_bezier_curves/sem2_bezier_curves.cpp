#include "../genericFunctions.h"

using namespace std;

double F(double x) {
    return x * x / 6 + cos(x + sin(x)) * x;
}

void displayGraph(double a, double b, double step, double interpolation_muliplier);

int main() {
    int a = -7, b = 10;
    int choice;
    bool balancedByStep = true;
    double step = 1;
    int temp;
    int interpolation_muliplier = 2;
    while (true) {
        clearScreen();
        coutWithColor(colors::LIGHT_BLUE, "Текущий промежуток: " + to_string(a) + " - " + to_string(b) + "\n");
        coutWithColor(colors::LIGHT_BLUE, "Текущий множитель интерполяции: " + to_string(interpolation_muliplier) + " (" 
            + to_string((int)((b - a) / step) + 1) + " точек до интерполяции, " + to_string((int)((b - a) / step) * interpolation_muliplier + 1) + " после)\n");
        coutWithColor(colors::LIGHT_BLUE, "Текущий шаг: " + to_string(step) + "\n");
        coutWithColor(colors::LIGHT_GREEN, "Балансирова по " +  string(balancedByStep ? "шагу" : "количеству точек") + " \n");
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        choice = displaySelection(new string[6]{
                "1.Ввести промежуток",
                "2.Ввести количество известных точек",
                "3.Ввести шаг",
                "4.Ввести множитель интерполяции",
                "5.Показать график + таблицу",
                "6.Выйти"
            }, 6);
        switch (choice) {
        case 1:

            a = inputData("Введите начало промежутка: ", false);
            b = inputData("Введите конец промежутка: ", false, [a](int input) -> bool {return input > a;}, 
                "Конец должен быть больше начала, повторите ввод: ");

            if (!balancedByStep) {
                step = (b - a) / (double)(temp - 1);
            }
            break;
        case 2:
            temp = inputData("Введите количество известных точек: ", false, [](int input) -> bool {return input >= 2;}, 
                "Слишком маленькое количество известных точек, повторите ввод: ");

            step = (b - a) / (double)(temp - 1);
            balancedByStep = false;
            break;
        case 3:
            step = inputData("Введите шаг: ", false, [a, b](int input) -> bool {return input < (b - a) / 2;}, 
                "Слишком большой шаг, повторите ввод: ");

            balancedByStep = true;
            break;
        case 4:
            interpolation_muliplier = inputData("Введите множитель: ", false, strictPositive,
                "Множитель должен быть больше 0, повторите ввод: ");
            break;
        case 5:
            displayGraph(a, b, step, interpolation_muliplier);
            break;
        case 6:
            return 0;
        }
    }
}

void displayTableOrig(vector<Vector2> points) {
    vector<string> I, X, Y;
    for (int i = 0; i < points.size(); i++) {
        I.push_back(to_string(i + 1));
        X.push_back(to_string(points[i].getX()));
        Y.push_back(to_string(points[i].getY()));
    }
    printTable({ "I", "X", "Y" }, { I, X, Y });
}

void displayTableInterpolated(vector<Vector2> points, vector<bool> highlight_flags) {
    vector<string> I, X, Y_interpolated, Y_real, Y_delta, titles;
    double x, y, y_real;
    double max_deviation = 0;
    for (int i = 0; i < points.size(); i++) {
        x = points[i].getX();
        y = points[i].getY();
        y_real = F(x);
        I.push_back(to_string(i + 1));
        X.push_back(to_string(x));
        Y_interpolated.push_back(to_string(y));
        Y_real.push_back(to_string(y_real));
        Y_delta.push_back(to_string(abs(y - y_real)));
        max_deviation = max(max_deviation, abs(y - y_real));
    }
    printTable(
        { "I", "X", "Y*", "Y", "abs(Y-Y*)" },
        { I, X, Y_interpolated, Y_real, Y_delta }, highlight_flags,
        colors::YELLOW, colors::DEFAULT, colors::LIGHT_GREEN, "Максимальное отклонение: " + to_string(max_deviation));
}

void displayGraph(double a, double b, double step, double interpolation_muliplier) {
    vector<Vector2> points;
    vector<Vector2> points_interpolated;
    vector<bool> points_interpolated_highlight_flags;

    for (double i = a; i <= b; i += step) {
        points.push_back({ i, F(i) });
    }
    coutWithColor(colors::LIGHT_YELLOW, "До интерполяции\n");
    displayTableOrig(points);
    vector<Vector2> P_vector = getPVector(points);
    vector<Vector2> A_vector = solve_tridiagonal_bezier(P_vector);
    vector<Vector2> B_vector = getBVector(points, A_vector);

    CubicBezier bezier;

    int pointsSize = points.size();

    for (int p = 0; p < pointsSize - 1; p++) {
        bezier.set(points[p], A_vector[p], B_vector[p], points[p + 1]);
        system(("echo '" + to_string(points[p].getX()) + " " + to_string(points[p].getY()) + "' >> points_all").c_str());
        system(("echo '" + to_string(A_vector[p].getX()) + " " + to_string(A_vector[p].getY()) + "' >> points_all").c_str());
        system(("echo '" + to_string(B_vector[p].getX()) + " " + to_string(B_vector[p].getY()) + "' >> points_all").c_str());
        for (double t = 0; t < interpolation_muliplier + (p == pointsSize - 2) ? 1 : 0; t++) {
            points_interpolated.push_back(bezier.getPoint(t / interpolation_muliplier));
            points_interpolated_highlight_flags.push_back(t == 0 || t == interpolation_muliplier);

            system(("echo '" + to_string(A_vector[p].getX()) + " " + to_string(A_vector[p].getY()) + "' >> points_a").c_str());
            system(("echo '" + to_string(B_vector[p].getX()) + " " + to_string(B_vector[p].getY()) + "' >> points_b").c_str());

            system(("echo '"
                + to_string(bezier.getPoint(t / interpolation_muliplier).getX()) + " "
                + to_string(bezier.getPoint(t / interpolation_muliplier).getY()) + "' >> interpolated").c_str());
        }
    }

    coutWithColor(colors::LIGHT_YELLOW, "После интерполяции\n");
    displayTableInterpolated(points_interpolated, points_interpolated_highlight_flags);

    double x, y;
    for (int i = 0; i < points_interpolated.size(); i++) {
        x = points_interpolated[i].getX();
        y = F(x);
        system(("echo '"
            + to_string(x) + " "
            + to_string(y) + "' >> real").c_str());
    }

    system("echo 'plot \"interpolated\" with lines, \"real\" with lines, \"points_a\", \"points_b\", \"points_all\" with lines, 0' | gnuplot --persist");
    system("rm real");
    system("rm interpolated");
    system("rm points_a");
    system("rm points_b");
    system("rm points_all");
    waitForButtonInput("Нажмите любую кнопку, чтобы вернуться в меню\n");
}