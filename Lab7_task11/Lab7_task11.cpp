#include "Lab7_task11.h"
#include "../genericFunctions.h"
using namespace std;

int main()
{
    
    while(true){
        string input = trim(inputString("Введите английские слова, разделенные пробелом: ", "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM ")) + ' ';
        cout << endl;
        if (input.length() == 1) {
            coutWithColor(colors::RED, "Ошибка ввода: пустая строка или только пробелы, повторите ввод\n");
            continue;
        }
        unsigned int numberOfWords;
        string* words = split(&input, true, &numberOfWords);

        coutWithColor(colors::YELLOW, "\nСортировка по алфавиту: \n");
        alphabeticSort(words, numberOfWords);

        for (unsigned int i = 0; i < numberOfWords; i++) {
            cout << words[i] << endl;
        }
        cout << endl;
        delete[] words; 

        continueOrExit();
    }

    return 0;
}

void alphabeticSort(string* words, unsigned int numberOfWords) {
    int j = 0;
    bool swap = true;
    string temp;
    while (swap)
    {
        swap = false;
        j++;
        for (unsigned int l = 0; l < numberOfWords - j; l++)
        {
            if (words[l] > words[l + 1])
            {
                temp = words[l];
                words[l] = words[l + 1];
                words[l + 1] = temp;
                swap = true;
            }
        }
    }
}

