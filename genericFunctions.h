#include <sstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set( "utf-8" )

double inputData(std::string message) {
    std::cout << message << std::flush;
    double toReturn;
    while (!(std::cin >> toReturn)) {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Пожалуйста, используйте числа" << std::endl;
    }
    return toReturn;
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

std::string addSpaces(std::string input, int targetLength) {
    int spaces = targetLength - input.length();
    for (int i = 0; i < spaces; i++) {
        input.append(" ");
    }
    return input;
}

void continueOrExit() {
    std::cout << "Продолжить?" << std::endl;
    std::string input;
    std::cin >> input;
    if (!(input == "yes" || input == "y" || input == "1")) {
        exit(-15);
    }
}

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
