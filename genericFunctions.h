#include <sstream>

#include <string>
#include <locale>
#include <codecvt>

#include <limits>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using std::string, std::vector;

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

    Vector2 operator*(double b) {
        return { x * b, y * b };
    }

    Vector2 operator+(Vector2 b) {
        return { x + b.x, y + b.y };
    }

    Vector2 operator-(Vector2 b) {
        return { x - b.x, y - b.y };
    }

    void operator*=(double b) {
        x *= b;
        y *= b;
    }

    void operator-=(Vector2 b) {
        x -= b.x;
        y -= b.y;
    }

    Vector2 operator/(double b) {
        return { x / b, y / b };
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

    void set(Vector2 a_, Vector2 b_, Vector2 c_, Vector2 d_) {
        a = a_;
        b = b_;
        c = c_;
        d = d_;
    }

    Vector2 getPoint(double t) {
        return
            a * pow(1 - t, 3) +
            b * 3 * t * pow(1 - t, 2) +
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

COORD getConsoleDimensions() {
#ifdef __linux__
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return { w.ws_col, w.ws_row };
#elif _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return {rows, columns};
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

void coutWithColor(colors color, string message, int delay = 0)
{
    setConsoleColor(color);
    std::cout << message << std::flush;
    setConsoleColor(colors::DEFAULT);
#ifdef __linux__
    usleep(delay * 1000);
#elif _WIN32
    Sleep(delay);
#endif
}

void coutWithColorAtPos(colors color, string message, int x, int y, int delay = 0) {
    setConsoleCursorPosition(x, y);
    coutWithColor(color, message, delay);
}

string* split(string* input, bool print_count, unsigned int* len) {
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

    string* words = new string[numberOfWords];
    int pos = 0;
    unsigned int index = 0;

    while ((pos = (*input).find(' ')) != string::npos) {
        if (pos > 0) {
            words[index] = (*input).substr(0, pos);
            index++;
        }
        (*input).erase(0, pos + 1);
    }
    return words;
}

string displayWarningWithInput(colors color, string message) {
    coutWithColor(color, message);
    string input;
    std::cin >> input;
    return input;
}

double inputData(string message, bool allowWhiteSpaces) {
    std::cout << message << std::flush;
    double toReturn;
    while (!(std::cin >> toReturn) || (std::cin.get() != '\n' && !allowWhiteSpaces)) {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Пожалуйста, используйте числа" << std::endl;
    }
    return toReturn;
}

double inputData(string message) {
    return inputData(message, true);
}

string inputData(string message, char* allowedChars, int allowedChars_size, std::regex pattern, string previousBuffer) {
    printf("%s", (message + previousBuffer).c_str());
    string buffer = previousBuffer;
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

string inputData(string message, char* allowedChars, int allowedChars_size, string previousBuffer) {
    std::regex str_expr(".*");
    return inputData(message, allowedChars, allowedChars_size, str_expr, previousBuffer);
}

string inputData(string message, char* allowedChars, int allowedChars_size, std::regex pattern) {
    return inputData(message, allowedChars, allowedChars_size, pattern, "");
}

string inputData(string message, char* allowedChars, int allowedChars_size) {
    std::regex str_expr(".*");
    return inputData(message, allowedChars, allowedChars_size, str_expr);
}

string doubleToString(double value, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;
    string strOut = out.str();
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

string doubleToString(double value) {
    return doubleToString(value, 5);
}

string addSymbols(string input, unsigned int targetLength, string symbol, bool center = false) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    int spaces = targetLength - converter.from_bytes(input).length();
    if (center) {
        int left = spaces / 2;
        int right = spaces - left;
        for (int i = 0; i < left; i++) {
            input.insert(0, symbol);
        }
        for (int i = 0; i < right; i++) {
            input.append(symbol);
        }
    } else {
        for (int i = 0; i < spaces; i++) {
            input.append(symbol);
        }
    }
    return input;
}

string addSpaces(string input, unsigned int targetLength) {
    return addSymbols(input, targetLength, " ");
}

string ltrim(const string s) {
    return std::regex_replace(s, std::regex("^\\s+"), string(""));
}

string rtrim(const string s) {
    return std::regex_replace(s, std::regex("\\s+$"), string(""));
}

string trim(const string s) {
    return ltrim(rtrim(s));
}

void continueOrExit() {
    string input = displayWarningWithInput(colors::DEFAULT, "Продолжить?\n");
    if (!(input == "yes" || input == "y" || input == "1")) {
        exit(-15);
    }
}

double lerp(double from, double to, double progress) {
    return from * (1 - progress) + to * progress;
}

int displaySelection(string* options, int optionCount) {

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

bool* displayMultiSelection(string* options, int optionCount) {

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

vector<Vector2> getPVector(vector<Vector2> points) {
    vector<Vector2> P_vector;
    P_vector.push_back(points[0] + points[1] * 2);
    for (int i = 1; i < points.size() - 2; i++) {
        P_vector.push_back(points[i] * 4 + points[i + 1] * 2);
    }
    P_vector.push_back(points[points.size() - 2] * 8 + points[points.size() - 1]);
    return P_vector;
}

vector<Vector2> getBVector(vector<Vector2> points, vector<Vector2> A_vector) {
    vector<Vector2> B_vector;
    for (int i = 0; i < A_vector.size() - 1; i++) {
        B_vector.push_back(points[i + 1] * 2 - A_vector[i + 1]);
    }
    B_vector.push_back((A_vector[points.size() - 2] + points[points.size() - 1]) / 2);
    return B_vector;
}

vector<Vector2> solve_tridiagonal_bezier(vector<Vector2>& P_vector) {

    int size = P_vector.size();

    float* diagonal = new float[size];
    float* sub_diagonal = new float[size];
    float* super_diagonal = new float[size];

    for (int i = 0; i < size - 1; i++) {
        diagonal[i] = 4;
    }
    diagonal[0] = 2;
    diagonal[size - 1] = 7;

    for (int i = 1; i < size - 1; i++) {
        sub_diagonal[i] = 1;
    }
    sub_diagonal[size - 1] = 2;

    for (int i = 0; i < size - 1; i++) {
        super_diagonal[i] = 1;
    }

    float* a = sub_diagonal;
    float* b = diagonal;
    float* c = super_diagonal;

    /*
     solves Ax = v where A is a tridiagonal matrix consisting of vectors a, b, c
     P_vector - initially contains the input vector v, and returns the solution x. indexed from 0 to X - 1 inclusive
     a - subdiagonal (means it is the diagonal below the main diagonal), indexed from 1 to X - 1 inclusive
     b - the main diagonal, indexed from 0 to X - 1 inclusive
     c - superdiagonal (means it is the diagonal above the main diagonal), indexed from 0 to X - 2 inclusive
     */

    c[0] = c[0] / b[0];
    P_vector[0] = P_vector[0] / b[0];

    /* loop from 1 to X - 1 inclusive, performing the forward sweep */
    for (size_t ix = 1; ix < P_vector.size(); ix++) {
        const float m = 1.0f / (b[ix] - a[ix] * c[ix - 1]);
        c[ix] = c[ix] * m;
        P_vector[ix] = (P_vector[ix] - P_vector[ix - 1] * a[ix]) * m;
    }

    /* loop from X - 2 to 0 inclusive (safely testing loop condition for an unsigned integer), to perform the back substitution */
    for (size_t ix = P_vector.size() - 1; ix-- > 0; ) {
        P_vector[ix] -= P_vector[ix + 1] * c[ix];
    }

    delete[] diagonal;
    delete[] sub_diagonal;
    delete[] super_diagonal;

    return P_vector;
}

void printGraph(vector<Converter> graphs, double from, double to, double step, int field_size = 45, GraphingBackend graphingBackend = GraphingBackend::CONSOLE, Interpolation interpolation = Interpolation::LINEAR, string gnuplotTitle = "") {
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
        coutWithColor(colors::LIGHT_BLUE, "Executing system command: " + plotString + "\n");
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

void printLayer(vector<int> column_widths, vector<string> column_elems,
    string start, string middle, string end,
    colors border_color, colors text_color, string fillWith = " ") {
    bool noElems = column_elems.empty();
    for (int column = 0; column < column_widths.size(); column++) {
        coutWithColor(border_color, column == 0 ? start : middle);
        coutWithColor(text_color, addSymbols(noElems ? "" : column_elems[column], column_widths[column], fillWith, true));
    }
    coutWithColor(border_color, end + "\n");
}

void printTable(vector<string> titles, vector<vector<string>> columns, vector<bool> highlight_flags = {},
    colors border_color = colors::YELLOW, colors text_color = colors::DEFAULT,
    colors highlight_color = colors::LIGHT_GREEN,
    string bottom_info = "", colors bottom_info_color = colors::CYAN) {
    using namespace std;

    int columns_len = columns.size();
    int elems = columns[0].size();

    for (int column = 1; column < columns_len; column++) {
        if (elems != columns[column].size()) {
            coutWithColor(colors::LIGHT_RED, "Несоотвествие размера колонок\n");
        }
        elems = min(elems, (int)columns[column].size());
    }

    vector<int> column_widths;
    for (int column = 0; column < columns_len; column++) {
        int maxWidth = 0;
        for (int elem = 0; elem < elems; elem++) {
            maxWidth = max(maxWidth, (int)columns[column][elem].length());
        }
        maxWidth = max((int)titles[column].length(), maxWidth) + 2;
        if ((maxWidth - titles[column].length()) % 2 != 0) {
            maxWidth--;
        }
        column_widths.push_back(maxWidth);
    }

    printLayer(column_widths, {    }, "╭", "┬", "╮", border_color, border_color, "─");
    printLayer(column_widths, titles, "│", "│", "│", border_color, text_color);
    printLayer(column_widths, {    }, "├", "┼", "┤", border_color, border_color, "─");

    bool highlight = !highlight_flags.empty();
    for (int elem = 0; elem < elems; elem++) {
        for (int column = 0; column < columns_len; column++) {
            coutWithColor(border_color, "│");
            coutWithColor(highlight && highlight_flags[elem] ? highlight_color : text_color, addSpaces(columns[column][elem], column_widths[column]));
        }
        coutWithColor(border_color, "│\n");
    }

    if (!bottom_info.empty()) {
        int fullWidth = 0;
        for (auto& n : column_widths)
            fullWidth += n;
        fullWidth += column_widths.size() - 1;
        printLayer(column_widths, {    }, "├", "┴", "┤", border_color, border_color, "─");
        coutWithColor(border_color, "│");
        coutWithColor(bottom_info_color, addSpaces(bottom_info, fullWidth));
        coutWithColor(border_color, "│\n");
        printLayer(column_widths, {    }, "╰", "─", "╯", border_color, border_color, "─");
    } else {
        printLayer(column_widths, {    }, "╰", "┴", "╯", border_color, border_color, "─");
    }

}