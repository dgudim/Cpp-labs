
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

//#define DEBUG

string current_expression;

const char precedenceLevels = 3;
char precedenceLevelOperatorCounts[precedenceLevels] = { 1, 2, 2 };
char operators[precedenceLevels][2] = { {'^'}, {'*', '/'}, {'+', '-'} };

const char alphabetChars = 26;
char alphabet[alphabetChars] =           { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
char alphabet_uppercase[alphabetChars] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

const char digitChars = 10;
char digits[digitChars] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const char functionCount = 14;
const char maxFunctionLength = 4;
string functions[functionCount] = {
    "abs", "sqrt",
    "sin", "cos", "tan", "ctg",
    "asin", "acos", "atan", "actg",
    "log", "exp", "cbrt", "ln"};

int variableCount = 0;
string user_variables[100];
double user_variable_values[100];

int costantsCount = 0;
string constants[100];
double constant_values[100];

unsigned int exceptionCount = 0;

string doubleToString(double value)
{
    ostringstream out;
    out.precision(15);
    out << std::fixed << value;
    string strOut = out.str();
    char currChar = strOut[strOut.length() - 1];
    while (currChar == '0' || currChar == '.') {
        strOut.erase(strOut.length() - 1, 1);
        currChar = strOut[strOut.length() - 1];
    }
    return strOut;
}

char charToLowerCase(char input) {
    for (int i = 0; i < alphabetChars; i++) {
        if (input == alphabet_uppercase[i]) {
            return alphabet[i];
        }
    }
    return input;
}

void stringToLowerCase(string& input) {
    for (int i = 0; i < input.length(); i++) {
        input[i] = charToLowerCase(input[i]);
    }
}

void printResult(long double result) {
    cout << "\n\tResult is " + doubleToString(result);
}

bool charIsPrecedentOperator(char character, char precedenceLevel) {
    for (int i = 0; i < precedenceLevelOperatorCounts[precedenceLevel]; i++) {
        if (operators[precedenceLevel][i] == character) {
            return true;
        }
    }
    return false;
}

bool charIsOperator(char input) {
    for (int i = 0; i < precedenceLevels; i++) {
        if (charIsPrecedentOperator(input, i)) {
            return true;
        }
    }
    return false;
}

bool charIsOperatorExceptMinus(char input) {
    if (input == '-') {
        return false;
    }
    return charIsOperator(input);
}

bool charIsLetter(char input) {
    for (int i = 0; i < alphabetChars; i++) {
        if (input == alphabet[i]) {
            return true;
        }
    }
    return false;
}

bool charIsDigit(char input) {
    for (int i = 0; i < digitChars; i++) {
        if (input == digits[i]) {
            return true;
        }
    }
    return false;
}

bool containsLetters(string input) {
    for (unsigned int i = 0; i < input.length(); i++) {
        if (charIsLetter(input[i])) {
            return true;
        }
    }
    return false;
}

double stringToDouble(string input) {
    if (containsLetters(input)) {
        throw "Contains letters, can't convert to double";
    }
    else {
        return stod(input);
    }
}

bool charIsDigitOrLetter(char input) {
    return charIsLetter(input) || charIsDigit(input);
}

bool charIsContextOperator(string expression, int index) {
    return (charIsDigitOrLetter(expression[index - 1]) || expression[index - 1] == ')') && (charIsDigitOrLetter(expression[index + 1]) || expression[index + 1] == '(' || expression[index + 1] == '-');
}

bool charIsAllowed(char input) {
    return charIsDigit(input) || charIsLetter(input) || charIsOperator(input) || input == '(' || input == ')' || input == '.';
}

bool hasParenthesis(string input) {
    for (unsigned int i = 0; i < input.length(); i++) {
        if (input[i] == '(' || input[i] == ')') {
            return true;
        }
    }
    return false;
}

void addUserVariable(string varName) {
    bool duplicate = false;
    for (int i = 0; i < variableCount; i++) {
        if (varName == user_variables[i]) {
            duplicate = true;
            break;
        }
    }
    if (!duplicate) {
        cout << "Found variable: " + varName << endl;
        user_variables[variableCount] = varName;
        variableCount++;
    }
}

void addConstant(string name, double value) {
    cout << "Added costant: " + name << endl;
    constants[costantsCount] = name;
    constant_values[costantsCount] = value;
    costantsCount++;
}

double getConstantOrVariableValue(string name) {
    for (int i = 0; i < costantsCount; i++) {
        if (constants[i] == name) {
            return constant_values[i];
        }
    }
    for (int i = 0; i < variableCount; i++) {
        if (user_variables[i] == name) {
            return user_variable_values[i];
        }
    }
    return 0;
}

bool isConstant(string name) {
    for (int i = 0; i < costantsCount; i++) {
        if (constants[i] == name) {
            return true;
        }
    }
    return false;
}

double inputData(string message) {
    cout << message;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please use numbers" << endl;
    }
    return toReturn;
}

double processOperator(char operatorChar, double arg1, double arg2) {
    switch (operatorChar) {
    case '+':
        return arg1 + arg2;
        break;
    case '-':
        return arg1 - arg2;
        break;
    case '/':
        if (arg2 == 0) {
            throw  "Division by zero";
        }
        return arg1 / arg2;
        break;
    case '*':
        return arg1 * arg2;
        break;
    case '^':
        return pow(arg1, arg2);
        break;
    default:
        cout << "Unknown operator " << operatorChar;
        return 0;
    }
}

double processFunction(int function, double arg) {
    switch (function) {
    case 0: 
        return abs(arg);
        break;
    case 1:
        if (arg < 0) {
            throw  "Can't extract square root of a negative number";
        }
        return sqrt(arg);
        break;
    case 2: 
        return sin(arg);
        break;
    case 3: 
        return cos(arg);
        break;
    case 4: 
        return tan(arg);
        break;
    case 5:
        return 1.0 / tan(arg);
        break;
    case 6: 
        if (arg > 1 || arg < -1) {
            throw  "Asin argument is out of range";
        }
        return asin(arg);
        break;
    case 7:
        if (arg > 1 || arg < -1) {
            throw "Acos argument is out of range";
        }
        return acos(arg);
        break;
    case 8: 
        return atan(arg);
        break;
    case 9:
        return atan(1.0/arg);
        break;
    case 10: 
        if (arg <= 0) {
            throw  "Log argument is out of range";
        }
        return log10(arg);
        break;
    case 11: 
        return exp(arg);
        break;
    case 12:
        return cbrt(arg);
        break;
    case 13:
        if (arg <= 0) {
            throw "Log argument is out of range";
        }
        return log(arg);
        break;
    case -1:
    default:
        if (function != -1) {
            cout << "Function not specified for index " << function << endl;
        }
        return arg;
        break;
    }
}

string getFunctionName(string expression, int parenthesisIndex) {
    string functionBuffer = "";
    if (parenthesisIndex == 0) {
        return functionBuffer;
    }
    for (unsigned int f = 0; f < maxFunctionLength; f++) {

        if (expression[parenthesisIndex - 1 - f] == '(' || charIsOperator(expression[parenthesisIndex - 1 - f])) {
            break;
        }
        functionBuffer.insert(0, 1, expression[parenthesisIndex - 1 - f]);
        if (parenthesisIndex - 1 - f == 0) {
            break;
        }
    }
    return functionBuffer;
}

int getFunctionIndex(string function) {
    for (int i = 0; i < functionCount; i++) {
        if (functions[i] == function) {
            return i;
        }
    }
    return -1;
}

bool isValidFunction(string function) {
    return  function == "" || getFunctionIndex(function) != -1;
}

void incrementExceptions() {
    exceptionCount++;
    if (exceptionCount == 9) {
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t >__< please stop \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
        exceptionCount = 0;
    }
}

double parseElementary(string expression) {
    #ifdef DEBUG
      cout << "Elementary parsing " + expression << endl;
    #endif
    double arg1 = 0;
    double arg2 = 0;
    int operatorIndex = -1;

    if (expression.length() > 2) {
        if (expression[1] == '-' && expression[0] == '-') {
            expression.erase(0, 2);
        }
    }

    string buffer = "";
    for (unsigned int i = 0; i < expression.length(); i++) {
        buffer.push_back(expression[i]);
        if (buffer.length() > 3) {
            buffer.erase(0, 1);
            if (buffer == "---") {
                expression.erase(i - 2, 2);
            }
        }
    }

    for (unsigned int i = 0; i < expression.length(); i++) {
        if (charIsOperatorExceptMinus(expression[i])) {
            operatorIndex = i;
            break;
        }
    }

    if (operatorIndex == -1) {
        for (unsigned int i = expression[0] == '-' ? 1 : 0; i < expression.length(); i++) {
            if (charIsOperator(expression[i])) {
                operatorIndex = i;
                break;
            }
        }
    }

    string arg1_string = expression.substr(0, operatorIndex);
    string arg2_string = expression.substr(operatorIndex + 1, expression.length() - 1);
    try
    {
        arg1 = stringToDouble(arg1_string);
    }
    catch (...)
    {
        arg1 = getConstantOrVariableValue(arg1_string);
    }

    try
    {
        arg2 = stringToDouble(arg2_string);
    }
    catch (...)
    {
        arg2 = getConstantOrVariableValue(arg2_string);
    }

    return processOperator(expression[operatorIndex], arg1, arg2);
}

string parseComplex(string input, string functionName) {
    int precedenceLevelOperatorCounts_inExpression[3] = {0, 0, 0};
    int operatorsLeft = 0;
    for (unsigned int i = 1; i < input.length() - 1; i++) {
        for (int i2 = 0; i2 < precedenceLevels; i2++) {
            if (charIsPrecedentOperator(input[i], i2) && charIsContextOperator(input, i)) {
                precedenceLevelOperatorCounts_inExpression[i2] ++;
                operatorsLeft++;
            }
        }
    }

    if (operatorsLeft == 0) {
        precedenceLevelOperatorCounts_inExpression[precedenceLevels - 1] ++;
        operatorsLeft++;
        input.append("+0");
    }

    for (int precedenceLevel = 0; precedenceLevel < precedenceLevels; precedenceLevel++) {
        while (precedenceLevelOperatorCounts_inExpression[precedenceLevel] > 0) {
            #ifdef DEBUG
                cout << "Calculating precedence level " << precedenceLevel << ", " << precedenceLevelOperatorCounts_inExpression[precedenceLevel] << " left" << endl;
            #endif // DEBUG
            int lastOperatorIndex = -1;
            for (unsigned int i = 1; i < input.length() - 1; i++) {
                if (charIsPrecedentOperator(input[i], precedenceLevel) && charIsContextOperator(input, i)) {
                    int lastIndex = input.length();
                    for (unsigned int i2 = i + 1; i2 < input.length() - 1; i2++) {
                        if (charIsOperator(input[i2]) && charIsContextOperator(input, i2)) {
                            lastIndex = i2;
                            break;
                        }
                    }
                    string toParse = input.substr(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1);
                    if (functionName.length() > 0) {
                    #ifdef DEBUG
                        cout << "Using function " << functionName << endl;
                    #endif
                    }
                    double parsedValue = processFunction(getFunctionIndex(functionName), parseElementary(toParse));
                    
                    input.replace(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1, doubleToString(parsedValue));
                    operatorsLeft--;
                    precedenceLevelOperatorCounts_inExpression[precedenceLevel] --;
                    cout << input << endl;
                    break;
                }
                if (charIsOperator(input[i]) && charIsContextOperator(input, i)) {
                    lastOperatorIndex = i;
                }
            }
        }
    }
    return input;
}

void processDeepestExpresion() {
    int firstIndex = 0;
    int lastIndex = current_expression.length() - 1;
    string functionName = "";

    string substr;

    for (unsigned int i = 0; i < current_expression.length(); i++) {
        if (current_expression[i] == '(') {
            firstIndex = i;
            functionName = getFunctionName(current_expression, i);
        }
        if (current_expression[i] == ')') {
            lastIndex = i;
            if (lastIndex - firstIndex == 1) {
                substr = "(0)";
            }
            else {
                substr = current_expression.substr(firstIndex + 1, lastIndex - firstIndex - 1);
            }
            break;
        }
    }
    string value = parseComplex(substr, functionName);
    current_expression.replace(firstIndex - functionName.length(), lastIndex - firstIndex + 1 + functionName.length(), value);
}

void calculate(string expressionStr) {
    current_expression = expressionStr;
    unsigned int parenthesisPair = 0;
    while (hasParenthesis(current_expression)) {
        cout << "Parenthesis level: " << parenthesisPair << endl;
        processDeepestExpresion();
        parenthesisPair ++;
    }
    printResult(stod(current_expression));
}

int main()
{
    cout << "Adding constants..." << endl;
    addConstant("pi", M_PI);
    addConstant("pi2", M_PI_2);
    addConstant("pi4", M_PI_4);
    addConstant("1pi", M_1_PI);
    addConstant("2pi", M_2_PI);
    addConstant("e", M_E);
    while (true) {
        try {
        string expression;
        variableCount = 0;
        current_expression = "";
        cout << "\nInput your expression" << endl;
        cin >> expression;
        stringToLowerCase(expression);
        cout << "Lowercased: " + expression << endl;
        cout << "Parsing..." << endl;
        cout << "Step 1: validity check" << endl;
        if (expression.length() == 0) {
            throw "Can't parse expression, length is 0";
        }
        if (expression.length() == 1) {
            if (charIsOperator(expression[0])) {
                throw "Can't parse expression, expression is an operator";
            }
        }

        for (unsigned int i = 0; i < expression.length(); i++) {
            if (!charIsAllowed(expression[i])) {
                throw "Can't parse expression, illegal character";
            }
        }

        int numberOfLeftParenthesis = 0;
        int numberOfRightParenthesis = 0;
        bool hasLetters = false;
        int operatorCount = false;
        for (unsigned int i = 0; i < expression.length(); i++) {
            if (expression[i] == '(') {
                numberOfLeftParenthesis++;
            }
            if (expression[i] == ')') {
                numberOfRightParenthesis++;
            }
            if (charIsOperator(expression[i])) {
                operatorCount++;
            }
            if (!hasLetters && charIsLetter(expression[i])) {
                hasLetters = true;
            }
        }
        if (!(numberOfLeftParenthesis == numberOfRightParenthesis)) {
            throw (string("Can't parse expression, ") + string((numberOfLeftParenthesis > numberOfRightParenthesis) ? "parenthesis opened and not closed" : "closed a non-existent parenthesis"));
        }

        if (!hasLetters && (operatorCount == 0 || (expression[0] == '-' && operatorCount == 1)) && numberOfLeftParenthesis == 0) {
            try {
                printResult(stod(expression));
            }
            catch (...) {
                throw  "Expression is not a number, who knew...";
            }
        }

        expression.push_back(')');
        expression.insert(0, "(");

        char lastChar = expression[0];
        for (unsigned int i = 1; i < expression.length(); i++) {
            if (operatorCount > 0) {
                if (i < expression.length() - 1) {
                    if ((charIsDigit(lastChar) || lastChar == ')') && expression[i] == '-' && charIsOperatorExceptMinus(expression[i + 1])) {
                        throw "Operator conflict at index " + to_string(i);
                    }
                }

                if ((charIsOperator(lastChar) && expression[i] == ')') || (lastChar == '(' && charIsOperator(expression[i]) && !(charIsDigit(expression[i + 1]) || expression[i + 1] == '('))) {
                    throw "Hanging operator";
                }
            }
            if (((lastChar == ')' || charIsDigit(lastChar) || charIsLetter(lastChar)) && expression[i] == '(') || (lastChar == ')' && (charIsDigit(expression[i]) || charIsLetter(expression[i])))) {

                string functionName = getFunctionName(expression, i);
                bool match = isValidFunction(functionName);

                if (!match) {
                    if (functionName.length() > 0) {
                        throw "Unknown function " + functionName;
                    }
                    throw "Can't parse expression, missing operator at index " + to_string(i);
                }

            }
            lastChar = expression[i];
        }

        
        cout << "Step 2: variable detection" << endl;
        string variableBuffer = "";
        for (unsigned int i = 0; i < expression.length(); i++) {
            if ((charIsOperator(expression[i]) || expression[i] == ')') && variableBuffer.length() != 0) {
                if (containsLetters(variableBuffer) && !isConstant(variableBuffer)) {
                    addUserVariable(variableBuffer);
                }
                variableBuffer.clear();
            }
            if (charIsDigitOrLetter(expression[i])) {
                variableBuffer.push_back(expression[i]);
            }
            else {
                variableBuffer.clear();
            }
        }

        if (variableBuffer.length() != 0) {
            addUserVariable(variableBuffer);
            variableBuffer.clear();
        }

        cout << "Step 3: variable input" << endl;
        for (int i = 0; i < variableCount; i++) {
            user_variable_values[i] = inputData("Please input value for " + user_variables[i] + " = ");
        }

        cout << "Step 4: actual calculation" << endl;
        calculate(expression);
        }
        catch (const char* msg) {
            cerr << msg << endl;
            incrementExceptions();
        }
        catch (string msg2) {
            cerr << msg2 << endl;
            incrementExceptions();
        }
        catch (...) {
            cerr << "Unhandled exception" << endl;
            cerr << "\n\n\n\n\nCongratulation! You have caused an unhandled exception, tell the developer))\n\n\n\n\n" << endl;
            incrementExceptions();
        }
    }
}
