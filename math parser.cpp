
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
#include <limits>
#include <string>

string current_expression;

const char precedenceLevels = 3;
char precedenceLevelOperatorCounts[precedenceLevels] = { 1, 2, 2 };
char operators[precedenceLevels][2] = { {'^'}, {'*', '/'}, {'+', '-'} };

const char alphabetChars = 26;
char alphabet[alphabetChars] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

const char digitChars = 10;
char digits[digitChars] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const char functionCount = 13;
const char maxFunctionLength = 4;
string functions[functionCount] = {
    "abs", "sqrt",
    "sin", "cos", "tan", "ctg",
    "asin", "acos", "atan", "actg",
    "log", "exp", "cbrt"};

int variableCount = 0;
string user_variables[100];
double user_variable_values[100];

int printResult(long double result) {
    cout << "\n\tResult is: " + to_string(result);
    return 1;
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
            cout << "Found duplicate variable: " + varName << endl;
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
            cout << "Division by zero" << endl;
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
            cout << "Can't extract square root of a negative number" << endl;
            return -100;
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
            cout << "Asin argument is out of range" << endl;
            return -100;
        }
        return asin(arg);
        break;
    case 7:
        if (arg > 1 || arg < -1) {
            cout << "Acos argument is out of range" << endl;
            return -100;
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
            cout << "Log argument is out of range" << endl;
        }
        return log10(arg);
        break;
    case 11: 
        return exp(arg);
        break;
    case 12:
        return cbrt(arg);
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

double parseElementary(string expression) {
    cout << expression << endl;
    double arg1 = 0;
    double arg2 = 0;
    int operatorIndex = -1;
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
        arg1 = stold(arg1_string);
    }
    catch (...)
    {
        for (int i = 0; i < variableCount; i++) {
            if (user_variables[i] == arg1_string) {
                arg1 = user_variable_values[i];
                break;
            }
        }
    }

    try
    {
        arg2 = stold(arg2_string);
    }
    catch (...)
    {
        for (int i = 0; i < variableCount; i++) {
            if (user_variables[i] == arg2_string) {
                arg2 = user_variable_values[i];
                break;
            }
        }
    }

    return processOperator(expression[operatorIndex], arg1, arg2);
}

string parseComplex(string input, string functionName) {
    int precedenceLevelOperatorCounts_inExpression[3] = {0, 0, 0};
    int operatorsLeft = 0;
    for (unsigned int i = 0; i < input.length(); i++) {
        for (int i2 = 0; i2 < precedenceLevels; i2++) {
            if (charIsPrecedentOperator(input[i], i2)) {
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
            cout << "Calculating precedence level " << precedenceLevel << ", " << precedenceLevelOperatorCounts_inExpression[precedenceLevel] << " left" << endl;
            int lastOperatorIndex = -1;
            for (unsigned int i = 0; i < input.length(); i++) {
                if (charIsPrecedentOperator(input[i], precedenceLevel)) {
                    int lastIndex = input.length();
                    for (unsigned int i2 = i + 1; i2 < input.length(); i2++) {
                        if (charIsOperator(input[i2])) {
                            lastIndex = i2;
                            break;
                        }
                    }
                    string toParse = input.substr(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1);
                    if (functionName.length() > 0) {
                        cout << "Using function " << functionName << endl;
                    }
                    input.replace(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1, to_string(processFunction(getFunctionIndex(functionName), parseElementary(toParse))));
                    operatorsLeft--;
                    precedenceLevelOperatorCounts_inExpression[precedenceLevel] --;
                    cout << input << endl;
                    break;
                }
                if (charIsOperator(input[i])) {
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
    for (unsigned int i = 0; i < current_expression.length(); i++) {
        if (current_expression[i] == '(') {
            firstIndex = i;
            functionName = getFunctionName(current_expression, i);
        }
        if (current_expression[i] == ')') {
            lastIndex = i;
            break;
        }
    }
    string value = parseComplex(current_expression.substr(firstIndex + 1, lastIndex - firstIndex - 1), functionName);
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
    printResult(stold(current_expression));
}

int main()
{
    string expression;
    cout << "Input your expression" << endl;
    cin >> expression;
    cout << "Parsing..." << endl;
    cout << "Step 1: validity check" << endl;
    if (expression.length() == 0) {
        cout << "Can't parse expression, length is 0" << endl;
        return -1;
    }
    if (expression.length() == 1) {
        if (charIsOperator(expression[0])) {
            cout << "Can't parse expression, expression is an operator" << endl;
            return -2;
        }
    }

    for (unsigned int i = 0; i < expression.length(); i++) {
        if (!charIsAllowed(expression[i])) {
            cout << "Can't parse expression, illegal character " << expression[i] << endl;
            return -100;
        }
    }

    int numberOfLeftParenthesis = 0;
    int numberOfRightParenthesis = 0;
    bool hasLetters = false;
    bool hasOperators = false;
    for (unsigned int i = 0; i < expression.length(); i++) {
        if (expression[i] == '(') {
            numberOfLeftParenthesis++;
        }
        if (expression[i] == ')') {
            numberOfRightParenthesis++;
        }
        if (!hasOperators && charIsOperator(expression[i])) {
            hasOperators = true;
        }
        if (!hasLetters && charIsLetter(expression[i])) {
            hasLetters = true;
        }
    }
    if (!(numberOfLeftParenthesis == numberOfRightParenthesis)) {
        cout << string("Can't parse expression, ") + string((numberOfLeftParenthesis > numberOfRightParenthesis) ? "parenthesis opened and not closed" : "closed a non-existent parenthesis") << endl;
        return -3;
    }

    if (!hasLetters && !hasOperators && numberOfLeftParenthesis == 0) {
        try {
            return printResult(stold(expression));
        }
        catch (...) {
            cout << "Expression is not a number, who knew..." << endl;
        }
    }

    expression.push_back(')');
    expression.insert(0, "(");

    char lastChar = expression[0];
    for (unsigned int i = 1; i < expression.length(); i++) {
        if (hasOperators) {
            if (charIsOperator(lastChar) && charIsOperator(expression[i])) {
                cout << "Can't parse expression, 2 or more subsequent operators" << endl;
                return -4;
            }
            if (charIsOperator(lastChar)) {
                bool hanging = (i == 1) || expression[i] == ')';
                if (i > 1) {
                    if (expression[i - 2] == '(') {
                        hanging = true;
                    }
                }
                if (hanging) {
                    cout << "Can't parse expression, hanging operator " << lastChar << endl;
                    //return -5;
                }
            }
            if (i == expression.length() - 1 && charIsOperator(expression[i])) {
                cout << "Can't parse expression, hanging operator " << lastChar << endl;
                return -5;
            }
        }
        if (((lastChar == ')' || charIsDigit(lastChar) || charIsLetter(lastChar)) && expression[i] == '(') || (lastChar == ')' && (charIsDigit(expression[i]) || charIsLetter(expression[i])))) {

            string functionName = getFunctionName(expression, i);
            bool match = isValidFunction(functionName);

            if (!match) {
                if (functionName.length() > 0) {
                    cout << "Unknown function " + functionName << endl;
                    return -6;
                }
                cout << "Can't parse expression, missing operator at index " + to_string(i) << endl;
                return -7;
            }
            
        }
        lastChar = expression[i];
    }

    cout << "Step 2: variable detection" << endl;
    string variableBuffer = "";
    for (unsigned int i = 0; i < expression.length(); i++) {
        if ((charIsOperator(expression[i]) || expression[i] == ')') && variableBuffer.length() != 0) {
            addUserVariable(variableBuffer);
            variableBuffer.clear();
        }
        if (charIsLetter(expression[i])) {
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

    //cout << parseElementary("-6+3") << " debug" << endl;

    cout << "Step 4: actual calculation" << endl;
    calculate(expression);

    return 0;
}
