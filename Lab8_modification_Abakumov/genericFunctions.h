#include <sstream>
#include <string>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set("utf-8")

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

void clearScreen() {
     system("CLS");
}

void setConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setConsoleColor(colors color){
    setConsoleColor((int) color);
}

void coutWithColor(int color, std::string message)
{
    setConsoleColor(color);
    std::cout << message << std::flush;
    setConsoleColor(colors::DEFAULT);
}

void coutWithColor(colors color, std::string message)
{
    coutWithColor((int)color, message);
}

void setConsoleCursorPosition(int x, int y)
{
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

std::string *split(std::string *input, bool print_count, unsigned int *len)
{
    unsigned int numberOfWords = 0;
    unsigned int strLen = (*input).length();
    for (unsigned int i = 0; i < strLen; i++)
    {
        if ((*input)[i] == ' ' && (*input)[i - 1] != ' ')
        {
            numberOfWords++;
        }
    }

    if (print_count)
    {
        coutWithColor(colors::LIGHTER_BLUE, "Number of words: " + std::to_string(numberOfWords));
    }

    (*len) = numberOfWords;

    std::string *words = new std::string[numberOfWords];
    int pos = 0;
    unsigned int index = 0;

    while ((pos = (*input).find(' ')) != std::string::npos)
    {
        if (pos > 0)
        {
            words[index] = (*input).substr(0, pos);
            index++;
        }
        (*input).erase(0, pos + 1);
    }
    return words;
}

std::string displayWarningWithInput(colors color, std::string message)
{
    coutWithColor(color, message);
    std::string input;
    std::cin >> input;
    return input;
}

double inputData(std::string message, bool allowWhiteSpaces)
{
    std::cout << message << std::flush;
    double toReturn;
    while (!(std::cin >> toReturn) || (std::cin.get() != '\n' && !allowWhiteSpaces))
    {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Please use numbers" << std::endl;
    }
    return toReturn;
}

double inputData(std::string message)
{
    return inputData(message, true);
}

std::string inputData(std::string message, char *allowedChars, int allowedChars_size, std::regex pattern, std::string previousBuffer, bool hide_input)
{
    printf("%s", (message + previousBuffer).c_str());
    std::string buffer = previousBuffer;
    while (true)
    {
        char currChar = _getch();
        bool addToBuffer = false;
        if (currChar == (int) keys::ENTER)
        {
            if (!std::regex_match(buffer, pattern))
            {
                coutWithColor(colors::YELLOW, "\nData doesn't match template\n");
                std::cout << buffer;
            }
            else
            {
                break;
            }
        }
        if (currChar == (int) keys::BACKSPACE)
        {
            unsigned int bufLen = buffer.length();
            if (bufLen > 0)
            {
                printf("%s", "\b \b");
                buffer.erase(bufLen - 1, bufLen);
            }
        }

        for (int i = 0; i < allowedChars_size; i++)
        {
            if (allowedChars[i] == currChar)
            {
                addToBuffer = true;
                putchar(hide_input ? '*' : currChar);
            }
        }

        if (addToBuffer)
        {
            buffer += currChar;
            addToBuffer = false;
        }
    }
    putchar('\n');
    return buffer;
}

std::string inputData(std::string message, char *allowedChars, int allowedChars_size, std::string previousBuffer)
{
    std::regex str_expr(".*");
    return inputData(message, allowedChars, allowedChars_size, str_expr, previousBuffer, false);
}

std::string inputData(std::string message, char *allowedChars, int allowedChars_size, std::regex pattern)
{
    return inputData(message, allowedChars, allowedChars_size, pattern, "", false);
}

std::string inputPassword()
{
    std::regex str_expr(".*");
    return inputData("Enter password: ", new char[76]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890!@#$%^&*()+" }, 75, str_expr, "", true);
}

std::string inputData(std::string message, char *allowedChars, int allowedChars_size)
{
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
    while ((currChar == '0' || currChar == '.') && strOut.length() > 1)
    {
        strOut.erase(strOut.length() - 1, 1);
        if (currChar == '.')
        {
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

std::string addSpaces(std::string input, unsigned int targetLength)
{
    int spaces = targetLength - input.length();
    for (int i = 0; i < spaces; i++)
    {
        input.append(" ");
    }
    return input;
}

std::string ltrim(const std::string s)
{
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string s)
{
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string s)
{
    return ltrim(rtrim(s));
}

void continueOrExit()
{
    std::string input = displayWarningWithInput(colors::DEFAULT, "Continue?\n");
    if (!(input == "yes" || input == "y" || input == "1"))
    {
        exit(-15);
    }
}

int displaySelection(std::string *options, int optionCount)
{

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int key = 0;

    for (int i = 0; i < optionCount; i++)
    {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true)
    {
        if (key == (int)keys::ARROW_UP)
        {
            counter--;
            if (counter < 0)
            {
                counter = optionCount - 1;
            }
        }
        if (key == (int)keys::ARROW_DOWN)
        {
            counter++;
            if (counter > optionCount - 1)
            {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++)
        {
            if (abs(counter - i) <= 1 || i == 0 || i == optionCount - 1)
            {
                setConsoleCursorPosition(0, offset + i);
                setConsoleColor(counter == i ? colors::LIGHT_RED : colors::DEFAULT);
                puts(options[i].c_str());
                fflush(stdout);
            }
        }
        key = _getch();
        if (key == (int) keys::ENTER)
        {
            coutWithColor(colors::GRAY, "\nYou've selected: " + options[counter] + "\n");
            return counter + 1;
        }
    }
}

bool *displayMultiSelection(std::string *options, int optionCount)
{

    bool *selectedFunctions = new bool[optionCount];
    for (int i = 0; i < optionCount; i++)
    {
        selectedFunctions[i] = false;
    }

    int offset = getConsoleCursorPosition().Y;
    int counter = 0;
    int key = 0;

    for (int i = 0; i < optionCount; i++)
    {
        setConsoleCursorPosition(0, offset + i);
        puts(options[i].c_str());
        fflush(stdout);
    }

    while (true)
    {
        if (key == (int )keys::ARROW_UP)
        {
            counter--;
            if (counter < 0)
            {
                counter = optionCount - 1;
            }
        }
        if (key == (int) keys::ARROW_DOWN)
        {
            counter++;
            if (counter > optionCount - 1)
            {
                counter = 0;
            }
        }
        for (int i = 0; i < optionCount; i++)
        {
            if (abs(counter - i) <= 1 || i == 0 || i == optionCount - 1)
            {
                setConsoleCursorPosition(0, offset + i);
                if (selectedFunctions[i])
                {
                    setConsoleColor(counter == i ? colors::LIGHT_GREEN : colors::GREEN);
                }
                else
                {
                    setConsoleColor(counter == i ? colors::LIGHT_RED : colors::DEFAULT);
                }
                puts(options[i].c_str());
                fflush(stdout);
            }
        }
        key = _getch();
        if (key == (int) keys::ENTER)
        {
            return selectedFunctions;
        }
        if (key == (int) keys::BACKSPACE)
        {
            selectedFunctions[counter] = !selectedFunctions[counter];
        }
    }
}
