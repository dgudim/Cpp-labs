#include <iostream>
using namespace std;

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

bool isNumOrLetter(char ch) {
    return isalpha(ch) || isupper(ch) || isdigit(ch);
}

bool tokenIsNumber(string& token) {
    for (int i = 0; i < token.size(); i++) {
        if (!isNumOrLetter(token.at(i))) {
            return false;
        }
    }
    return true;
}

int main() {
    string testString = "a * ( b + baba * kek) + 666 ^ 7";
    Tokenizer* tokenizer = new Tokenizer(testString);
    cout << "Parsing string: " << testString << endl;
    string currentToken;
    try {
        while (tokenizer->hasTokens()) {
            currentToken = tokenizer->getNextToken();

        }
    } catch (const char* err) {
        cout << "Runtime error: " << err << endl;
    } catch (string err) {
        cout << "Runtime error: " << err << endl;
    }
}