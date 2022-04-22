#include <sstream>
#include <string>
#include <limits>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef __linux__
#include <termios.h>
#include <unistd.h>
int _getch(void)
{
    struct termios oldattr, newattr;
    int ch;

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int get_pos(int* y, int* x)
{

    char buf[30] = { 0 };
    int ret, i, pow;
    char ch;

    *y = 0;
    *x = 0;

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for (i = 0, ch = 0; ch != 'R'; i++)
    {
        ret = read(0, &ch, 1);
        if (!ret)
        {
            tcsetattr(0, TCSANOW, &restore);
            fprintf(stderr, "getpos: error reading response!\n");
            return 1;
        }
        buf[i] = ch;
        // printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
    }

    if (i < 2)
    {
        tcsetattr(0, TCSANOW, &restore);
        // printf("i < 2\n");
        return (1);
    }

    for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + (buf[i] - '0') * pow;

    for (i--, pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + (buf[i] - '0') * pow;

    tcsetattr(0, TCSANOW, &restore);
    return 0;
}

struct COORD
{
    int X, Y;
    COORD(int x, int y)
    {
        X = x;
        Y = y;
    }
};

#elif _WIN32
#include <conio.h>
#define NOMINMAX
#include <windows.h>
#endif

#pragma execution_character_set("utf-8")

constexpr int colors_count = 12;

#ifdef __linux__
enum class keys
{
    ARROW_UP = 65,
    ARROW_DOWN = 66,
    ENTER = '\n',
    BACKSPACE = 127
};
enum class colors
{
    DEFAULT = 39,
    WHITE = 97,
    GRAY = 90,
    BLACK = 30,

    BLUE = 34,
    LIGHT_BLUE = 94,
    LIGHTER_BLUE = 96,
    CYAN = 36,

    LIGHT_RED = 91,
    RED = 31,

    LIGHT_YELLOW = 93,
    YELLOW = 33,

    LIGHT_GREEN = 92,
    GREEN = 32,

    LIGHT_PURPLE = 95,
    PURPLE = 35
};
#elif _WIN32
enum class keys
{
    ARROW_UP = 72,
    ARROW_DOWN = 80,
    ENTER = 13,
    BACKSPACE = '\b'
};
enum class colors
{
    DEFAULT = 7,
    WHITE = 15,
    GRAY = 8,
    BLACK = 0,

    BLUE = 1,
    LIGHT_BLUE = 9,
    LIGHTER_BLUE = 3,
    CYAN = 11,

    LIGHT_RED = 12,
    RED = 4,

    LIGHT_YELLOW = 14,
    YELLOW = 6,

    LIGHT_GREEN = 10,
    GREEN = 2,

    LIGHT_PURPLE = 13,
    PURPLE = 5
};
#endif

class Vector2 {
private:
    double x, y;
public:
    Vector2(double x_, double y_) {
        x = x_;
        y = y_;
    }

    Vector2() {
        x = 0;
        y = 0;
    }

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }

    Vector2 operator+(Vector2 b) {
        return { x + b.x, y + b.y };
    }

    Vector2 operator-(Vector2 b) {
        return { x - b.x, y - b.y };
    }

    Vector2 operator*(double b) {
        return { x * b, y * b };
    }

    void operator*=(double b) {
        x *= b;
        y *= b;
    }

    Vector2 operator/(double b) {
        return { x / b, y / b };
    }

    Vector2 operator*(Vector2 b) {
        return { x * b.x, y * b.y };
    }

    Vector2 operator/(Vector2 b) {
        return { x / b.x, y / b.y };
    }
};

class CubicBezier {
private:
    Vector2 a, b, c, d;
public:
    CubicBezier(Vector2 a_ = {}, Vector2 b_ = {}, Vector2 c_ = {}, Vector2 d_ = {}) {
        a = a_;
        b = b_;
        c = c_;
        d = d_;
    }

    Vector2 getPoint(double t) {
        return
            a * pow(1 - t, 3) +
            b * 3 * pow(1 - t, 2) * t +
            c * 3 * (1 - t) * pow(t, 2) +
            d * pow(t, 3);
    }
};

enum class GraphingBackend {
    CONSOLE,
    GNUPLOT
};

enum class Interpolation {
    LINEAR,
    BEZIER
};

typedef double (*Converter)(double);

constexpr colors colormap[colors_count] = {
    colors::LIGHT_GREEN, colors::GREEN,
    colors::CYAN, colors::LIGHTER_BLUE,
    colors::LIGHT_BLUE, colors::BLUE,
    colors::LIGHT_PURPLE, colors::PURPLE,
    colors::LIGHT_YELLOW, colors::YELLOW,
    colors::LIGHT_RED, colors::RED };

void setConsoleCursorPosition(int x, int y) {
#ifdef __linux__
    std::cout << "\033[" << y << ";" << x << "H";
#elif _WIN32
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#endif
}

COORD getConsoleCursorPosition() {
#ifdef __linux__
    int x = 0;
    int y = 0;
    get_pos(&y, &x);
    return { x, y };
#elif _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
#endif
}

colors mapToColor(int n, int min = 0, int max = colors_count - 1, bool loop = true) {
    max = std::max(min, max);
    if (!loop) {
        n = std::clamp(n, min, max);
    }
    max -= min;
    n -= min;
    min = 0;
    if (loop) {
        n -= n / max * max;
    }
    return colormap[(int)((n / (float)max) * (colors_count - 1))];
}

void clearScreen() {
#ifdef __linux__
    system("clear");
#elif _WIN32
    system("CLS");
#endif
}

void setConsoleColor(int color)
{
#ifdef __linux__
    std::cout << "\033[" << color << "m";
#elif _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

void setConsoleColor(colors color) {
    setConsoleColor((int)color);
}

void coutWithColor(colors color, std::string message, int delay = 0)
{
    setConsoleColor(color);
    std::cout << message << std::flush;
    setConsoleColor(colors::DEFAULT);
    usleep(delay * 1000);
}

void coutWithColorAtPos(colors color, std::string message, int x, int y, int delay = 0) {
    setConsoleCursorPosition(x, y);
    coutWithColor(color, message, delay);
}

std::string* split(std::string* input, bool print_count, unsigned int* len) {
    unsigned int numberOfWords = 0;
    unsigned int strLen = (*input).length();
    for (unsigned int i = 0; i < strLen; i++) {
        if ((*input)[i] == ' ' && (*input)[i - 1] != ' ') {
            numberOfWords++;
        }
    }

    if (print_count) {
        coutWithColor(colors::LIGHTER_BLUE, "Количество слов: " + std::to_string(numberOfWords));
    }

    (*len) = numberOfWords;

    std::string* words = new std::string[numberOfWords];
    int pos = 0;
    unsigned int index = 0;

    while ((pos = (*input).find(' ')) != std::string::npos) {
        if (pos > 0) {
            words[index] = (*input).substr(0, pos);
            index++;
        }
        (*input).erase(0, pos + 1);
    }
    return words;
}

std::string displayWarningWithInput(colors color, std::string message) {
    coutWithColor(color, message);
    std::string input;
    std::cin >> input;
    return input;
}

double inputData(std::string message, bool allowWhiteSpaces) {
    std::cout << message << std::flush;
    double toReturn;
    while (!(std::cin >> toReturn) || (std::cin.get() != '\n' && !allowWhiteSpaces)) {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Пожалуйста, используйте числа" << std::endl;
    }
    return toReturn;
}

double inputData(std::string message) {
    return inputData(message, true);
}

std::string inputData(std::string message, char* allowedChars, int allowedChars_size, std::regex pattern, std::string previousBuffer) {
    printf("%s", (message + previousBuffer).c_str());
    std::string buffer = previousBuffer;
    while (true) {
        char currChar = _getch();
        bool addToBuffer = false;
        if (currChar == (int)keys::ENTER) {
            if (!std::regex_match(buffer, pattern)) {
                coutWithColor(colors::YELLOW, "\nВведеные данные не соответствуют шаблону\n");
                std::cout << buffer;
            } else {
                break;
            }
        }
        if (currChar == (int)keys::BACKSPACE) {
            unsigned int bufLen = buffer.length();
            if (bufLen > 0) {
                printf("%s", "\b \b");
                buffer.erase(bufLen - 1, bufLen);
            }
        }

        for (int i = 0; i < allowedChars_size; i++) {
            if (allowedChars[i] == currChar) {
                addToBuffer = true;
                putchar(currChar);
                break;
            }
        }

        if (addToBuffer) {
            buffer += currChar;
            addToBuffer = false;
        }
    }
    putchar('\n');
    return buffer;
}

std::string inputData(std::string message, char* allowedChars, int allowedChars_size, std::string previousBuffer) {
    std::regex str_expr(".*");
    return inputData(message, allowedChars, allowedChars_size, str_expr, previousBuffer);
}

std::string inputData(std::string message, char* allowedChars, int allowedChars_size, std::regex pattern) {
    return inputData(message, allowedChars, allowedChars_size, pattern, "");
}

std::string inputData(std::string message, char* allowedChars, int allowedChars_size) {
    std::regex str_expr(".*");
    return inputData(message, allowedChars, allowedChars_size, str_expr);
}

std::string doubleToString(double value, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;
    std::string strOut = out.str();
    char currChar = strOut[strOut.length() - 1];
    while ((currChar == '0' || currChar == '.') && strOut.length() > 1) {
        strOut.erase(strOut.length() - 1, 1);
        if (currChar == '.') {
            break;
        }
        currChar = strOut[strOut.length() - 1];
    }
    return strOut;
}

std::string doubleToString(double value) {
    return doubleToString(value, 5);
}

std::string addSpaces(std::string input, unsigned int targetLength) {
    int spaces = targetLength - input.length();
    for (int i = 0; i < spaces; i++) {
        input.append(" ");
    }
    return input;
}

std::string ltrim(const std::string s) {
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string s) {
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string s) {
    return ltrim(rtrim(s));
}

void continueOrExit() {
    std::string input = displayWarningWithInput(colors::DEFAULT, "Продолжить?\n");
    if (!(input == "yes" || input == "y" || input == "1")) {
        exit(-15);
    }
}

double lerp(double from, double to, double progress) {
    return from * (1 - progress) + to * progress;
}

int displaySelection(std::string* options, int optionCount) {

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int prev_counter = 0;
    int key = 0;

    for (int i = 0; i < optionCount; i++) {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true) {

        if (key == (int)keys::ARROW_UP) {
            prev_counter = counter;
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        } else if (key == (int)keys::ARROW_DOWN) {
            prev_counter = counter;
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        } else if (key >= '1' && key <= '9') {
            prev_counter = counter;
            counter = std::clamp(key - '1', 0, optionCount - 1);
        }

        for (int i = 0; i < optionCount; i++) {
            if (i == counter || i == prev_counter) {
                setConsoleCursorPosition(0, offset + i);
                setConsoleColor(counter == i ? colors::LIGHT_RED : colors::DEFAULT);
                puts(options[i].c_str());
                fflush(stdout);
            }
        }

        key = _getch();
        if (key == (int)keys::ENTER) {
            setConsoleCursorPosition(0, offset + optionCount);
            coutWithColor(colors::GRAY, "\nВы выбрали: " + options[counter] + "\n");
            return counter + 1;
        }
    }
}

bool* displayMultiSelection(std::string* options, int optionCount) {

    bool* selectedFunctions = new bool[optionCount];
    for (int i = 0; i < optionCount; i++) {
        selectedFunctions[i] = false;
    }

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int prev_counter = 0;
    int key = 0;

    for (int i = 0; i < optionCount; i++) {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true) {
        if (key == (int)keys::ARROW_UP) {
            prev_counter = counter;
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        } else if (key == (int)keys::ARROW_DOWN) {
            prev_counter = counter;
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        } else if (key >= '1' && key <= '9') {
            prev_counter = counter;
            counter = std::clamp(key - '1', 0, optionCount - 1);
        }

        for (int i = 0; i < optionCount; i++) {
            if (i == counter || i == prev_counter) {
                setConsoleCursorPosition(0, offset + i);
                if (selectedFunctions[i]) {
                    setConsoleColor(counter == i ? colors::LIGHT_GREEN : colors::GREEN);
                } else {
                    setConsoleColor(counter == i ? colors::LIGHT_RED : colors::DEFAULT);
                }
                puts(options[i].c_str());
                fflush(stdout);
            }
        }

        key = _getch();
        if (key == (int)keys::ENTER) {
            setConsoleCursorPosition(0, offset + optionCount);
            return selectedFunctions;
        }
        if (key == (int)keys::BACKSPACE) {
            selectedFunctions[counter] = !selectedFunctions[counter];
        }
    }
}

Vector2 getCubicBezierACoef(std::vector<Vector2> points, int index) {
    Vector2 a;
    if (index == 0) {
        a = points[0] + points[1] * 2;
    } else {
        a = points[index] * 2 + points[index + 1];
        a *= 2;
    }
    return a;
}

Vector2 getCubicBezierBCoef(std::vector<Vector2> points, int index) {
    Vector2 b;
    if (index == points.size() - 2) {
        b = (getCubicBezierACoef(points, index) + points[index + 1]) / 2;
    } else {
        b = points[index + 1] * 2 - getCubicBezierACoef(points, index + 1);
    }
    return b;
}

void test() {
    using namespace std;
    vector<Vector2> points;

    points.push_back({ 1, 1 });
    points.push_back({ 2, 4 });
    points.push_back({ 4, 7 });
    points.push_back({ 9, 0 });

    CubicBezier bezier = {
        points[0],
    getCubicBezierACoef(points, 0),
    getCubicBezierBCoef(points, 0),
        points[1] };

    CubicBezier bezier2 = {
        points[1],
    getCubicBezierACoef(points, 1),
    getCubicBezierBCoef(points, 1),
        points[2] };

    for (double t = 0; t <= 100; t++) {
        cout << bezier.getPoint(t / 100).getX() << "    ";
        cout << bezier.getPoint(t / 100).getY() << endl;
    }
    for (double t = 0; t <= 100; t++) {
        cout << bezier2.getPoint(t / 100).getX() << "    ";
        cout << bezier2.getPoint(t / 100).getY() << endl;
    }
}

void printGraph(std::vector<Converter> graphs, double from, double to, double step, int field_size = 45, GraphingBackend graphingBackend = GraphingBackend::CONSOLE, Interpolation interpolation = Interpolation::LINEAR, std::string gnuplotTitle = "") {
    using namespace std;

    vector<vector<double>> x_points;
    vector<vector<double>> y_points;

    unsigned int point;

    double minX = from;
    double maxX = to;

    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    for (int g = 0; g < graphs.size(); g++) {
        vector<double> x;
        vector<double> y;
        point = 0;
        for (double i = from; i * (step < 0 ? -1 : 1) <= to * (step < 0 ? -1 : 1); i += step) {
            x.push_back(i);
            y.push_back(graphs[g](i));
            maxY = max(maxY, y.back());
            minY = min(minY, y.back());
            point++;
        }
        x_points.push_back(x);
        y_points.push_back(y);
    }

    if (graphingBackend == GraphingBackend::GNUPLOT) {
        string plotString;
        plotString = "echo 'set title \"" + gnuplotTitle + "\"; plot ";
        for (int g = 0; g < graphs.size(); g++) {
            for (int p = 0; p < point; p++) {
                system(("echo '" + to_string(x_points[g][p]) + " " + to_string(y_points[g][p]) + "' >> plot" + to_string(g)).c_str());
            }
            if (g > 0) {
                plotString += ", ";
            }
            plotString += "\"plot" + to_string(g) + "\" ";
            plotString += interpolation == Interpolation::BEZIER ? "smooth bezier" : "with lines";
        }
        plotString += "' | gnuplot --persist";
        system(plotString.c_str());
        for (int g = 0; g < graphs.size(); g++) {
            system(("rm plot" + to_string(g)).c_str());
        }
        return;
    }

    for (int g = 0; g < graphs.size(); g++) {
        for (unsigned int p = 0; p < point; p++) {
            x_points[g][p] -= minX;
            x_points[g][p] /= (maxX - minX);
            x_points[g][p] *= (field_size - 1);

            y_points[g][p] -= minY;
            y_points[g][p] /= (maxY - minY);
            y_points[g][p] *= (field_size - 1);
        }
    }

    int** field = new int* [field_size];
    for (unsigned int i = 0; i < field_size; i++) {
        field[i] = new int[field_size];
    }

    for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
        for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
            field[x_coord][y_coord] = 0;
        }
    }

    for (int g = 0; g < graphs.size(); g++) {
        for (unsigned int p = 0; p < point - 1; p++) {
            unsigned int steps = max(abs((int)x_points[g][p] - (int)x_points[g][p + 1]), abs((int)y_points[g][p] - (int)y_points[g][p + 1])) + 1;
            if (steps > 1) {
                for (unsigned int i = 1; i < steps; i++) {
                    field[(int)lerp(x_points[g][p], x_points[g][p + 1], i / (double)(steps - 1))][(int)lerp(y_points[g][p], y_points[g][p + 1], i / (double)(steps - 1))] += (g + 1);
                }
            } else {
                field[(int)x_points[g][p]][(int)y_points[g][p]] += (g + 1);
            }

        }
    }

    for (unsigned int y_coord = 0; y_coord < field_size; y_coord++) {
        for (unsigned int x_coord = 0; x_coord < field_size; x_coord++) {
            coutWithColor(field[x_coord][field_size - y_coord - 1] ?
                mapToColor(field[x_coord][field_size - y_coord - 1] - 1) : // graphs
                colors::BLACK, // background
                field[x_coord][field_size - y_coord - 1] > 0 ? "██" : "░░");
        }
        cout << endl;
    }

    for (unsigned int i = 0; i < field_size; ++i) {
        delete[] field[i];
    }
    delete[] field;
}