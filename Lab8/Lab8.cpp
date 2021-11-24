#include "Lab8.h"
#include "../genericFunctions.h"
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <map>
using namespace std;

enum lessons {
    PHYSICS = 0,
    MATH = 1,
    INFORMATICS = 2,
    CHEMISTRY = 3
};
int lessons_size = 4;
const char* lessons_map[] = { "Физика", "Математика", "Информатика", "Химия" };
const char* lessons_map_case[] = { "физике", "математике", "информатике", "химии" };

struct student_entry {
    string fio;
    unsigned int year_of_birth;
    unsigned int group;
    map<lessons, vector<int>> grades;
    float grades_average;
};

void inputEntry(student_entry *entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53);
    entry->year_of_birth = (unsigned int)inputData("Введите год рождения: ");
    entry->group = (unsigned int)inputData("Введите номер группы: ");
    for (int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел" << endl;
        string input = trim(inputData("", new char[12]{ "1234567890 " }, 11)) + ' ';
        unsigned int numberOfWords;
        string* words = split(&input, false, &numberOfWords);
        vector<int>* grades_input = new vector<int>;
        for (unsigned int i2 = 0; i2 < numberOfWords; i2++) {
           (*grades_input).push_back(stoi(input));
        }
        
    }
}

int main()
{
    student_entry entry;
    inputEntry(&entry);
}


