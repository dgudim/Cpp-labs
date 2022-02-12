#include <sstream>
#include <string>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__ 
    #include <termios.h>
    #include <unistd.h>
int _getch(void)
{
struct termios oldattr, newattr;
int ch;

tcgetattr( STDIN_FILENO, &oldattr );
newattr = oldattr;
newattr.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
ch = getchar();
tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
return ch;
}

int get_pos(int *y, int *x) {

 char buf[30]={0};
 int ret, i, pow;
 char ch;

*y = 0; *x = 0;

 struct termios term, restore;

 tcgetattr(0, &term);
 tcgetattr(0, &restore);
 term.c_lflag &= ~(ICANON|ECHO);
 tcsetattr(0, TCSANOW, &term);

 write(1, "\033[6n", 4);

 for( i = 0, ch = 0; ch != 'R'; i++ )
 {
    ret = read(0, &ch, 1);
    if ( !ret ) {
       tcsetattr(0, TCSANOW, &restore);
       fprintf(stderr, "getpos: error reading response!\n");
       return 1;
    }
    buf[i] = ch;
    printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
 }

 if (i < 2) {
    tcsetattr(0, TCSANOW, &restore);
    printf("i < 2\n");
    return(1);
 }

 for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
     *x = *x + ( buf[i] - '0' ) * pow;

 for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
     *y = *y + ( buf[i] - '0' ) * pow;

 tcsetattr(0, TCSANOW, &restore);
 return 0;
}

struct COORD{
    int X, Y;
    COORD(int x, int y){
        X = x;
        Y = y;
    }
};

#elif _WIN32
    #include <conio.h>
    #define NOMINMAX
    #include <windows.h>
#endif

#pragma execution_character_set( "utf-8" )

enum class colors {
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

void setConsoleColor(int color) {
    #ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    #endif
}

void coutWithColor(int color, std::string message) {
    setConsoleColor(color);
    std::cout << message << std::flush;
    setConsoleColor(7);
}

void coutWithColor(colors color, std::string message) {
    coutWithColor((int)color, message);
}

void setConsoleCursorPosition(int x, int y) {
    #ifdef __linux__ 
        std::cout << "\033[" << x << ";" << y << "H";
    #elif _WIN32
        COORD c;
        c.X = x;
        c.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    #endif
}

COORD getConsoleCursorPosition()
{
    #ifdef __linux__ 
        int x = 0;
        int y = 0;
        get_pos(&y, &x);
        return {x, y};
    #elif _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.dwCursorPosition;
    #endif
}

std::string* split(std::string *input, bool print_count,unsigned int *len) {
    unsigned int numberOfWords = 0;
    unsigned int strLen = (*input).length();
    for (unsigned int i = 0; i < strLen; i++) {
        if ((*input)[i] == ' ' && (*input)[i - 1] != ' ') {
            numberOfWords++;
        }
    }
   
    if (print_count) {
        coutWithColor(3, "Количество слов: " + std::to_string(numberOfWords));
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

std::string displayWarningWithInput(int color, std::string message) {
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
        if (currChar == '\r') {
            if (!std::regex_match(buffer, pattern)) {
                coutWithColor(6, "\nВведеные данные не соответствуют шаблону\n");
                std::cout << buffer;
            }
            else {
                break;
            }
        }
        if (currChar == '\b') {
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

std::string doubleToString(double value, int precision)
{
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

std::string doubleToString(double value)
{
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
    std::string input = displayWarningWithInput(7, "Продолжить?\n");
    if (!(input == "yes" || input == "y" || input == "1")) {
        exit(-15);
    }
}

int displaySelection(std::string* options, int optionCount) {

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int key = 0;

    for (int i = 0; i < optionCount; i++) {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true) {
        if (key == 72) {
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        }
        if (key == 80) {
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++) {
            if (abs(counter - i) <= 1 || i == 0 || i == optionCount - 1) {
                setConsoleCursorPosition(0, offset + i);
                setConsoleColor(counter == i ? 12 : 7);
                puts(options[i].c_str());
                fflush(stdout);
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
        }
        if (key == '\r') {
            coutWithColor(8, "\nВы выбрали: " + options[counter] + "\n");
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
    int key = 0;

    for (int i = 0; i < optionCount; i++) {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true) {
        if (key == 72) {
            counter--;
            if (counter < 0) {
                counter = optionCount - 1;
            }
        }
        if (key == 80) {
            counter++;
            if (counter > optionCount - 1) {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++) {
            if (abs(counter - i) <= 1 || i == 0 || i == optionCount - 1) {
                setConsoleCursorPosition(0, offset + i);
                if (selectedFunctions[i]) {
                    setConsoleColor(counter == i ? 10 : 2);
                }
                else {
                    setConsoleColor(counter == i ? 12 : 7);
                }
                puts(options[i].c_str());
                fflush(stdout);
            }
        }
        key = _getch();
        if (key == 224) {
            key = _getch();
        }
        if (key == '\r') {
            return selectedFunctions;
        }
        if (key == 8) {
            selectedFunctions[counter] = !selectedFunctions[counter];
        }
    }
}
