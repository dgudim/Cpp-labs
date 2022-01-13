#include "Lab7_task11_charArr.h"
#include "../genericFunctions.h"
using namespace std;

int main()
{
    SetConsoleOutputCP(65001);
    while (true) {
        char* input = inputData_char("Введите английские слова, разделенные пробелом: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM " }, 53, 121);
        cout << endl;

        unsigned int numberOfWords = -1;
        unsigned int strLen = strlen(input);
        for (unsigned int i = 0; i < strLen; i++) {
            if (input[i] == ' ' && input[i - 1] != ' ') {
                numberOfWords++;
            }
        }
        if (numberOfWords == 0) {
            coutWithColor(4, "Ошибка ввода: пустая строка или только пробелы, повторите ввод\n");
            continue;
        }
        coutWithColor(3, "Количество слов: " + to_string(numberOfWords));

        char** words = new char* [numberOfWords];
        int pos = 0;
        unsigned int lastIndex = -1;
        unsigned int index = 0;

        for (unsigned int i = 1; i < strLen; i++) {
            if (input[i] != ' ' && input[i - 1] == ' ') {
                if (lastIndex != i - 1 && lastIndex != -1) {
                    index = i - 1;
                    char* newStr = substr(input, lastIndex + 1, index);
                    bool valid = false;
                    size_t newStr_len = index - lastIndex;
                    for (size_t i2 = 0; i2 < newStr_len; i2++) {
                        if (newStr[i2] != ' ') {
                            valid = true;
                            break;
                        }
                    }
                    if (true) {
                        words[pos] = newStr;
                        cout << "\n" << newStr;
                        pos++;
                    }
                    else {
                        delete[] newStr;
                    }
                }
                lastIndex = i - 1;
            }
        }

        coutWithColor(6, "\nСортировка по алфавиту: \n");
        alphabeticSort_char(words, numberOfWords);

        for (unsigned int i = 0; i < numberOfWords; i++) {
            cout << words[i] << endl;
        }
        cout << endl;
        delete[] words;

        continueOrExit();
    }

    return 0;
}

char* substr(char* str, unsigned int start, unsigned int end) {
    char* sub = new char[end - start + 1];
    memcpy(sub, str + start, end - start);
    sub[end - start] = '\0';
    return sub;
}

int myStrcmp(char* s1, char* s2) {
    if (*s1 == *s2)
        return *s1 == '\0' ? 0 : myStrcmp(s1 + 1, s2 + 1);

    return (*s1 > *s2) ? 1 : -1;
    //myStrcmp(s1+i, s2+i)
}

bool compare2Words(char* word1, char* word2) {
    return (word1 && word2) ? 0 > myStrcmp(word1, word2) : word1 < word2;
}

void alphabeticSort_char(char** words, unsigned int numberOfWords) {
    int j = 0;
    bool swap = true;
    char* temp;
    while (swap)
    {
        swap = false;
        j++;
        for (unsigned int l = 0; l < numberOfWords - j; l++)
        {
            if (!compare2Words(words[l], words[l + 1]))
            {
                temp = words[l];
                words[l] = words[l + 1];
                words[l + 1] = temp;
                swap = true;
            }
        }
    }
}

char* inputData_char(const char* message, char* allowedChars, int allowedChars_size, const int maxStringSize) {
    printf("%s", message);

    char* buffer = new char[maxStringSize + 2];
    for (int i = 0; i < maxStringSize; i++) {
        buffer[i] = ' ';
    }
    buffer[maxStringSize] = 'a';
    buffer[maxStringSize + 1] = '\0';

    int buffLen = 1;
    char maxCharCodePoint = allowedChars[0];
    char minCharCodePoint = allowedChars[0];
    for (int i = 0; i < allowedChars_size; i++) {
        maxCharCodePoint = max(maxCharCodePoint, allowedChars[i]);
        minCharCodePoint = min(minCharCodePoint, allowedChars[i]);
    }
    while (true) {
        char currChar = _getch();
        bool addToBuffer = false;
        if (currChar == '\r') {
            break;
        }
        if (currChar == '\b') {
            if (buffLen > 1) {
                printf("%s", "\b \b");
                buffLen--;
                buffer[buffLen] = ' ';
            }
        }
        if (currChar >= minCharCodePoint && currChar <= maxCharCodePoint && buffLen < maxStringSize - 1) {
            for (int i = 0; i < allowedChars_size; i++) {
                if (allowedChars[i] == currChar) {
                    addToBuffer = true;
                    putchar(currChar);
                }
            }
        }
        if (addToBuffer) {
            buffer[buffLen] = currChar;
            buffLen++;
            addToBuffer = false;
        }
    }
    return buffer;
}

