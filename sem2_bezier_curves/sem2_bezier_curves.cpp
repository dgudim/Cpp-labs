#include "../genericFunctions.h"

using namespace std;

double F(double x) {
    return x * x / 6 + cos(x + sin(x)) * x;
}

void displayGraph(double a, double b, double step, double interpolation_muliplier);

int main() {
    double a = -7, b = 10;
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
            a = inputData("Введите a: ");
            while (true) {
                b = inputData("Введите b: ");
                if (b > a) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "b должен быть больше a, повторите ввод\n");
            }
            if (!balancedByStep) {
                step = (b - a) / temp;
            }
            break;
        case 2:
            while (true) {
                temp = inputData("Введите количество известных точек: ");
                if (temp >= 2) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Слишком маленькое количество известных точек, повторите ввод\n");
            }
            step = (b - a) / temp;
            balancedByStep = false;
            break;
        case 3:
            while (true) {
                step = inputData("Введите шаг: ");
                if (step < (b - a) / 2) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Слишком большой шаг, повторите ввод\n");
            }
            balancedByStep = true;
            break;
        case 4:
            while (true) {
                interpolation_muliplier = inputData("Введите множитель: ");
                if (interpolation_muliplier > 0) {
                    break;
                }
                coutWithColor(colors::LIGHT_RED, "Множитель должен быть больше 0, повторите ввод\n");
            }
            break;
        case 5:
            displayGraph(a, b, step, interpolation_muliplier);
            break;
        case 6:
            return 0;
        }
    }
}


void displayTable(vector<Vector2> points, vector<bool> points_interpolated_highlight_flags = {}) {
    bool hightlight_enabled = points_interpolated_highlight_flags.size() == points.size();
    coutWithColor(colors::YELLOW, "╭───┬───────────┬───────────╮\n");
    coutWithColor(colors::YELLOW, "│ I │     X     │     Y     │\n");
    coutWithColor(colors::YELLOW, "├───┼───────────┼───────────┤\n");
    colors currentColor = colors::DEFAULT;
    for (int i = 0; i < points.size(); i++) {
        currentColor = hightlight_enabled ?
            (points_interpolated_highlight_flags[i] ? colors::LIGHT_GREEN : colors::DEFAULT)
            : colors::DEFAULT;
        coutWithColor(colors::YELLOW, "│");
        coutWithColor(currentColor, addSpaces(to_string(i + 1), 3));
        coutWithColor(colors::YELLOW, "│");
        coutWithColor(currentColor, addSpaces(to_string(points[i].getX()), 11));
        coutWithColor(colors::YELLOW, "│");
        coutWithColor(currentColor, addSpaces(to_string(points[i].getY()), 11));
        coutWithColor(colors::YELLOW, "│\n");
    }
    coutWithColor(colors::YELLOW, "╰───┴───────────┴───────────╯\n");
}

void displayGraph(double a, double b, double step, double interpolation_muliplier) {
    vector<Vector2> points;
    vector<Vector2> points_interpolated;
    vector<bool> points_interpolated_highlight_flags;

    for (double i = a; i <= b; i += step) {
        points.push_back({ i, F(i) });
    }
    coutWithColor(colors::LIGHT_YELLOW, "До интерполяции\n");
    displayTable(points);
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
    displayTable(points_interpolated, points_interpolated_highlight_flags);

    double delta_error = 0;
    double x, y;
    for (int i = 0; i < points_interpolated.size(); i++) {
        x = points_interpolated[i].getX();
        y = F(x);
        delta_error = max(delta_error, abs(points_interpolated[i].getY() - y));
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
    coutWithColor(colors::CYAN, "Максимальное отклонение: " + to_string(delta_error) + "\n");
    coutWithColor(colors::LIGHTER_BLUE, "Нажмите любую кнопку, чтобы вернуться в меню\n");
    _getch();
}