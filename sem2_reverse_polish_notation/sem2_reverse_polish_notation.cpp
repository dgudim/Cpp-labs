#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <stdio.h>
#include <math.h>

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
    string buff;
public:
    Tokenizer(string string) {
        str = string;
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
                        throw "illegal character " + buff + "\n";
                    }

                    if (str.empty()) {
                        return buff;
                    }

                    ch = str.at(0);

                    if (isOperator(ch) || isParenthesis(ch)) {
                        return buff;
                    } else if (isParenthesis(ch)) {
                        throw "missing operator after " + buff + "\n";
                    }
                }
            } else if (isOperator(ch)) {
                buff.insert(0, 1, ch);
                str.erase(0, 1);
                skipSpaces();
                if (str.empty()) {
                    throw "dangling operator " + buff + "\n";
                }
                if (!isOperator(str.at(0))) {
                    return buff;
                } else {
                    throw "subsequent operators after " + buff + "\n";
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
                    throw "subsequent parenthesis after " + buff + "\n";
                }
            }
        }
        throw "can't get next token\n";
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
        return isalpha(ch) || isupper(ch) || isdigit(ch);
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
    return isLetter(ch) || isdigit(ch);
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
                cout << "RightParen error " << token.str.c_str() << "\n";
                return {};
            }
        }
        break;

        default:
            cout << "error, unknown token: " << token.str.c_str() << "\n";
            return {};
        }
    }

    // When there are no more tokens to read:
    // While there are still operator tokens in the stack:
    while (!stack.empty()) {
        // If the operator token on the top of the stack is a parenthesis,
        // then there are mismatched parentheses.
        if (stack.back().type == Token::Type::LeftParen) {
            cout << "Mismatched parentheses error\n";
            return {};
        }

        // Pop the operator onto the output queue.
        queue.push_back(move(stack.back()));
        stack.pop_back();
    }

    return queue;
}


int main() {

    string expr = "30 + 40 * 20 / (1 - 5) ^ 2 ^ 3 + -10";
    
    if (expr.at(0) == '-') {
        expr.insert(0, 1, '0');
    }
    Tokenizer* tokenizer = new Tokenizer(expr);
    string currentToken;
    try {
        while (tokenizer->hasTokens()) {
            currentToken = tokenizer->getNextToken();
        }
    } catch (const char* err) {
        cout << "Error: " << err << endl;
    } catch (string err) {
        cout << "Error: " << err << endl;
    }

    const auto tokens = exprToTokens(expr);
    auto queue = shuntingYard(tokens);

    vector<int> stack;

    cout << "\nCalculation\n";

    while (!queue.empty()) {
        string op;

        const Token token = queue.front();
        cout << token;
        queue.pop_front();
        switch (token.type) {
        case Token::Type::Number:
        case Token::Type::Variable:

            if (token.type == Token::Type::Variable) {
                stack.push_back(0);
            } else {
                stack.push_back(stoi(token.str));
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
                cout << "Operator error " << token.str.c_str() << endl;
                exit(0);
                break;
            case '^':
                stack.push_back(static_cast<int>(pow(lhs, rhs)));
                break;
            case '*':
                stack.push_back(lhs * rhs);
                break;
            case '/':
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
            cout << "Token error\n";
            exit(0);
        }
    }
    cout << "\n  result = " << stack.back() << endl;

}