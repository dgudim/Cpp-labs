#include <sstream>
#include <string>
#include <iostream>
#include <limits>

#pragma execution_character_set( "utf-8" )

double inputData(string message) {
    cout << message << flush;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while (cin.get() != '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

std::string doubleToString(double value)
{
    std::ostringstream out;
    out.precision(35);
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

bool continueOrExit() {
    std::cout << "Продолжить?" << std::endl;
    std::string input;
    std::cin >> input;
    return input == "yes" || input == "y" || input == "1";
}