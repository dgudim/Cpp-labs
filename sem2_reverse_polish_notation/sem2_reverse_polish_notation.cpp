#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <stdio.h>
#include <math.h>
#include "../genericFunctions.h"

using namespace std;

class Token {
public:
    enum class Type {
        Undefined,
        Number,
        Variable,
        Operator,
        LeftParen,
        RightParen,
    };

    Token(Type t, const string& s, int prec = -1, bool ra = false)
        : type{ t }, str(s), precedence{ prec }, rightAssociative{ ra } {}

    const Type type;
    const string str;
    const int precedence;
    const bool rightAssociative;
};

class Tokenizer {
private:
    string str;
    string str_copy;
    string buff;
public:
    Tokenizer(string string) {
        setExpression(string);
    }

    void setExpression(string string) {
        str = string;
        str_copy = string;
    }

    void reset() {
        str = str_copy;
    }

    bool hasTokens() {
        skipSpaces();
        return !str.empty();
    }

    string getNextToken() {
        skipSpaces();
        buff.clear();
        char ch = 0;
        if (!str.empty()) {
            ch = str.at(0);
            if (isNumOrLetter(ch)) {
                while (!str.empty()) {
                    buff.insert(buff.end(), 1, ch);
                    str.erase(0, 1);
                    skipSpaces();
                    if (!isNumOrLetter(ch)) {
                        throw "нелегальный символ " + buff;
                    }

                    if (str.empty()) {
                        return buff;
                    }

                    ch = str.at(0);

                    if (isOperator(ch) || isParenthesis(ch)) {
                        return buff;
                    } else if (isParenthesis(ch)) {
                        throw "не хватает оператора после " + buff;
                    }
                }
            } else if (isOperator(ch)) {
                buff.insert(0, 1, ch);
                str.erase(0, 1);
                skipSpaces();
                if (str.empty()) {
                    throw "не хватает второго значения после оператора: " + buff;
                }
                if (!isOperator(str.at(0))) {
                    return buff;
                } else {
                    throw "стоящие подряд операторы " + buff;
                }
            } else if (isParenthesis(ch)) {
                buff.insert(0, 1, ch);
                str.erase(0, 1);
                skipSpaces();
                if (str.empty()) {
                    return buff;
                }
                if (!isParenthesis(str.at(0))) {
                    return buff;
                } else {
                    if (ch != str.at(0)) {
                        throw "стоящие подряд разные скобки после " + buff;
                    } else {
                        return buff;
                    }
                }
            }
        }
        throw "не могу получить следующий токен";
    }

    void skipSpaces() {
        if (str.empty()) return;
        char ch = str.at(0);
        while (ch == ' ') {
            str.erase(0, 1);
            ch = str.at(0);
        }
    }

    bool isNumOrLetter(char ch) {
        return isalpha(ch) || isupper(ch) || isdigit(ch) || ch == '.';
    }

    bool isParenthesis(char ch) {
        return ch == '(' || ch == ')';
    }

    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
    }
};

ostream& operator<<(ostream& os, const Token& token) {
    os << token.str;
    return os;
}

bool isLetter(char ch) {
    return isalpha(ch) || isupper(ch);
}

bool isLetterOrNumber(char ch) {
    return isLetter(ch) || isdigit(ch) || ch == '.';
}

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

deque<Token> exprToTokens(const string& expr) {
    deque<Token> tokens;

    for (const auto* ch = expr.c_str(); *ch; ++ch) {
        if (isblank(*ch)) {
            // ignore
        } else if (isLetterOrNumber(*ch)) {
            const auto* b = ch;
            bool isVar = false;
            while (isLetterOrNumber(*ch)) {
                if (isLetter(*ch)) isVar = true;
                ++ch;
            }
            const auto str = string(b, ch);
            tokens.push_back(Token{ isVar ? Token::Type::Variable : Token::Type::Number, str });
            --ch;
        } else {
            Token::Type type = Token::Type::Undefined;
            int precedence = -1;
            bool ra = false;
            switch (*ch) {
            default:                                       break;
            case '(':   type = Token::Type::LeftParen;     break;
            case ')':   type = Token::Type::RightParen;    break;
            case '^':   type = Token::Type::Operator;      precedence = 4; ra = true;  break;
            case '*':   type = Token::Type::Operator;      precedence = 3; break;
            case '/':   type = Token::Type::Operator;      precedence = 3; break;
            case '+':   type = Token::Type::Operator;      precedence = 2; break;
            case '-':   type = Token::Type::Operator;      precedence = 2; break;
            }
            tokens.push_back(Token{ type, string(1, *ch), precedence, ra });
        }
    }

    return tokens;
}


deque<Token> shuntingYard(const deque<Token>& tokens) {
    deque<Token> queue;
    vector<Token> stack;

    for (Token token : tokens) {
        // Read a token
        switch (token.type) {
        case Token::Type::Number:
        case Token::Type::Variable:
            // If the token is a number or variable, then add it to the output queue
            queue.push_back(token);
            break;

        case Token::Type::Operator:
        {
            // If the token is operator, o1, then:
            const Token o1 = token;

            // while there is an operator token,
            while (!stack.empty()) {
                // o2, at the top of stack, and
                const Token o2 = stack.back();

                // either o1 is left-associative and its precedence is
                // *less than or equal* to that of o2,
                // or o1 if right associative, and has precedence
                // *less than* that of o2,
                if (
                    (!o1.rightAssociative && o1.precedence <= o2.precedence)
                    || (o1.rightAssociative && o1.precedence < o2.precedence)
                    ) {
                    // then pop o2 off the stack,
                    stack.pop_back();
                    // onto the output queue;
                    queue.push_back(o2);

                    continue;
                }

                // otherwise, exit.
                break;
            }

            // push o1 onto the stack.
            stack.push_back(o1);
        }
        break;

        case Token::Type::LeftParen:
            // If token is left parenthesis, then push it onto the stack
            stack.push_back(token);
            break;

        case Token::Type::RightParen:
            // If token is right parenthesis:
        {
            bool match = false;

            // Until the token at the top of the stack
            // is a left parenthesis,
            while (!stack.empty() && stack.back().type != Token::Type::LeftParen) {
                // pop operators off the stack
                // onto the output queue.
                queue.push_back(stack.back());
                stack.pop_back();
                match = true;
            }

            // Pop the left parenthesis from the stack,
            // but not onto the output queue.
            stack.pop_back();

            if (!match && stack.empty()) {
                // If the stack runs out without finding a left parenthesis,
                // then there are mismatched parentheses.
                throw "Ошибка закрывающей скобки " + token.str;
                return {};
            }
        }
        break;

        default:
            throw "Неизвестный токен: " + token.str;
            return {};
        }
    }

    // When there are no more tokens to read:
    // While there are still operator tokens in the stack:
    while (!stack.empty()) {
        // If the operator token on the top of the stack is a parenthesis,
        // then there are mismatched parentheses.
        if (stack.back().type == Token::Type::LeftParen) {
            cout << "Несоответствие скобок";
            return {};
        }

        // Pop the operator onto the output queue.
        queue.push_back(move(stack.back()));
        stack.pop_back();
    }

    return queue;
}

void calculate(deque<Token> queue) {

    vector<double> stack;
    map<string, double> var_map;
    double in_var = 0;

    while (!queue.empty()) {
        string op;

        const Token token = queue.front();

        queue.pop_front();
        switch (token.type) {
        case Token::Type::Number:
        case Token::Type::Variable:

            if (token.type == Token::Type::Variable) {
                if (var_map.find(token.str) != var_map.end()) {
                    stack.push_back(var_map.at(token.str));
                } else {
                    in_var = inputData("Введите значение для " + token.str + ": ");
                    var_map.insert(std::pair<string, double>(token.str, in_var));
                    stack.push_back(in_var);
                }
            } else {
                stack.push_back(stod(token.str));
            }
            break;

        case Token::Type::Operator:
        {
            const auto rhs = stack.back();
            stack.pop_back();
            const auto lhs = stack.back();
            stack.pop_back();

            switch (token.str[0]) {
            default:
                throw "Ошибка оператора " + token.str;
                break;
            case '^':
                if (lhs < 0 && rhs < 1 && rhs > 0) {
                    throw  "Возведение отрицательного числа в дробную степень";
                }
                if (lhs == 0 && rhs == 0) {
                    throw  "Возведение нуля в нулевую степень";
                }
                stack.push_back(pow(lhs, rhs));
                break;
            case '*':
                stack.push_back(lhs * rhs);
                break;
            case '/':
                if (rhs == 0) {
                    throw "Деление на 0 ";
                }
                stack.push_back(lhs / rhs);
                break;
            case '+':
                stack.push_back(lhs + rhs);
                break;
            case '-':
                stack.push_back(lhs - rhs);
                break;
            }
        }
        break;

        default:
            throw "Ошибка токена";
        }
    }
    clearScreen();
    coutWithColor(colors::LIGHT_BLUE, "Результат = " + to_string(stack.back()) + "\n");
}

int main() {

    string expr = "a/(b-c)*(d+e)";
    Tokenizer* tokenizer = new Tokenizer(expr);
    deque<Token> queue;
    string currentToken;
    bool exit = false;

    while (true) {
        coutWithColor(colors::LIGHT_GREEN, "Текущее выражение: " + expr + "\n");
        coutWithColor(colors::LIGHT_GREEN, "Текущая польская запись: ");
        for (Token token : queue) {
            cout << token;
        }
        cout << endl;
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[4]{
            "1.Ввести пример",
            "2.Преобразовать в польскую запись",
            "3.Решить",
            "4.Выйти" }, 4);

        switch (choise)
        {
        case 1:
            exit = false;
            while (!exit) {
                expr = inputData("Выражение: ", new char[72]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM. 1234567890+-/*^()" }, 71);

                if (expr.empty()) {
                    coutWithColor(colors::LIGHT_RED, "ошибка ввода, пустая строка\n");
                    continue;
                }

                if (expr.at(0) == '(' && expr.at(expr.size() - 1) == ')'){
                    expr.insert(0, "0+");
                }

                if (expr.at(0) == '-' || expr.at(0) == '+') {
                    expr.insert(0, 1, '0');
                } else if (isOperator(expr.at(0))) {
                    coutWithColor(colors::LIGHT_RED, "ошибка ввода, первый символ - оператор\n");
                } else {
                    exit = true;
                }
            }

            queue.clear();

            tokenizer->setExpression(expr);
            clearScreen();
            break;
        case 2:
            clearScreen();
            try {
                while (tokenizer->hasTokens()) {
                    currentToken = tokenizer->getNextToken();
                }
                queue = shuntingYard(exprToTokens(expr));
            } catch (const string str) {
                coutWithColor(colors::LIGHT_RED, str + "\n");
            } catch (const char* str) {
                coutWithColor(colors::LIGHT_RED, string(str) + "\n");
            } catch (...) {
                coutWithColor(colors::LIGHT_RED, "Неизвестная ошибка, последний токен: " + currentToken + "\n");
            }
            tokenizer->reset();
            break;
        case 3:
            try {
                if (queue.empty()) {
                    clearScreen();
                    coutWithColor(colors::LIGHT_RED, "Польская запись не сгенерирована\n");
                    break;
                }
                calculate(queue);
            } catch (const string str) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, str + "\n");
            } catch (const char* str) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, string(str) + "\n");
            } catch (...) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Неизвестная ошибка\n");
            }
            break;
        case 4:
            return 1;
        }
    }
}