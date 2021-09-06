
#include <iostream>
#include <math.h>
using namespace std;
#include <limits>
#include <string>

string expression;
string current_expression;

const char precedenceLevels = 3;
char precedenceLevelOperatorCounts[precedenceLevels] = { 1, 2, 2 };
char operators[precedenceLevels][2] = { {'^'}, {'*', '/'}, {'+', '-'} };

const char alphabetChars = 26;
char alphabet[alphabetChars] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

const char digitChars = 10;
char digits[digitChars] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const char functionCount = 14;
string functions[functionCount] = {
    "abs", "sqrt",
    "sin", "cos", "tan", "ctg",
    "asin", "acos", "atan", "actg",
    "ln", "log", "exp" };

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
    return charIsDigit(input) || charIsLetter(input) || charIsOperator(input) || input == '(' || input == ')';
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

double parseElementary(string expression) {
    double arg1 = 0;
    double arg2 = 0;
    int operatorIndex = 0;
    for (int i = 0; i < expression.length(); i++) {
        if (charIsOperator(expression[i])) {
            operatorIndex = i;
            string arg1_string = expression.substr(0, i);
            string arg2_string = expression.substr(i + 1, expression.length() - 1);
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

            break;
        }
    }
    return processOperator(expression[operatorIndex], arg1, arg2);
}

string parseComplex(string input) {
    int precedenceLevelOperatorCounts_inExpression[3] = {0, 0, 0};
    int operatorsLeft = 0;
    for (int i = 0; i < input.length(); i++) {
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
            // a/b/c/d+d*c*b*a-3
            cout << "Calculating level " << precedenceLevel << ", " << precedenceLevelOperatorCounts_inExpression[precedenceLevel] << " left" << endl;
            int lastOperatorIndex = -1;
            for (int i = 0; i < input.length(); i++) {
                if (charIsPrecedentOperator(input[i], precedenceLevel)) {
                    int lastIndex = input.length();
                    for (int i2 = i + 1; i2 < input.length(); i2++) {
                        if (charIsOperator(input[i2])) {
                            lastIndex = i2;
                            break;
                        }
                    }
                    string toParse = input.substr(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1);
                    input.replace(lastOperatorIndex + 1, lastIndex - lastOperatorIndex - 1, to_string(parseElementary(toParse)));
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

void processDeepestExpresion(string input) {
    int firstIndex = 0;
    int lastIndex = input.length() - 1;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '(') {
            firstIndex = i;
        }
        if (input[i] == ')') {
            lastIndex = i;
            break;
        }
    }
    string value = parseComplex(input.substr(firstIndex + 1, lastIndex - firstIndex - 1));
    input.replace(firstIndex, lastIndex - firstIndex + 1, value);
}

void calculate(string expressionStr) {
    processDeepestExpresion(expressionStr);
}

int main()
{
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

    for (int i = 0; i < expression.length(); i++) {
        if (!charIsAllowed(expression[i])) {
            cout << "Can't parse expression, illegal character " << expression[i] << endl;
            return -100;
        }
    }

    int numberOfLeftParenthesis = 0;
    int numberOfRightParenthesis = 0;
    bool hasLetters = false;
    bool hasOperators = false;
    for (int i = 0; i < expression.length(); i++) {
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
    for (int i = 1; i < expression.length(); i++) {
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
                    return -5;
                }
            }
            if (i == expression.length() - 1 && charIsOperator(expression[i])) {
                cout << "Can't parse expression, hanging operator " << lastChar << endl;
                return -5;
            }
        }
        if (((lastChar == ')' || charIsDigit(lastChar) || charIsLetter(lastChar)) && expression[i] == '(') || (lastChar == ')' && (charIsDigit(expression[i]) || charIsLetter(expression[i])))) {
            cout << "Can't parse expression, missing operator at index " + to_string(i) << endl;
            return -6;
        }
        lastChar = expression[i];
    }

    cout << "Step 2: variable detection" << endl;
    string variableBuffer = "";
    for (int i = 0; i < expression.length(); i++) {
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

    cout << "Step 4: actual calculation" << endl;
    calculate(expression);

    return 0;
}
