#include <sstream>
#include <string>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set( "utf-8" )

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void coutWithColor(int color, std::string message) {
    setConsoleColor(color);
    std::cout << message << std::flush;
    setConsoleColor(7);
}

void setConsoleCursorPosition(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

COORD getConsoleCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
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

std::string inputData(std::string message, char* allowedChars, int allowedChars_size) {
    printf("%s", message.c_str());
    std::string buffer = "";
    char maxCharCodePoint = allowedChars[0];
    char minCharCodePoint = allowedChars[0];
    for (int i = 0; i < allowedChars_size; i++) {
        maxCharCodePoint = std::max(maxCharCodePoint, allowedChars[i]);
        minCharCodePoint = std::min(minCharCodePoint, allowedChars[i]);
    }
    while (true) {
        char currChar = _getch();
        bool addToBuffer = false;
        if (currChar == '\r') {
            break;
        }
        if (currChar == '\b') {
            unsigned int bufLen = buffer.length();
            if (bufLen > 0) {
                printf("%s", "\b \b");
                buffer.erase(bufLen - 1, bufLen);
            }
        }
        if (currChar >= minCharCodePoint && currChar <= maxCharCodePoint) {
            for (int i = 0; i < allowedChars_size; i++) {
                if (allowedChars[i] == currChar) {
                    addToBuffer = true;
                    putchar(currChar);
                }
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

double inputData(std::string message) {
    return inputData(message, true);
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
