#include "Lab8.h"
#include "../genericFunctions.h"
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <map>
#include <fstream>
using namespace std;

enum lessons {
    PHYSICS = 0,
    MATH = 1,
    INFORMATICS = 2,
    CHEMISTRY = 3
};
unsigned int lessons_size = 4;
const char* lessons_map[] = { "Физика", "Математика", "Информатика", "Химия" };
const char* lessons_map_case[] = { "физике", "математике", "информатике", "химии" };

struct student_entry {
    string fio;
    unsigned int year_of_birth;
    unsigned int group;
    map<lessons, vector<unsigned int>> grades;
    float grades_average;
};

void inputEntry(student_entry *entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53);
    entry->year_of_birth = (unsigned int)inputData("Введите год рождения: ");
    entry->group = (unsigned int)inputData("Введите номер группы: ");
    unsigned int grades_sum = 0;
    unsigned int grades_count = 0;
    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел" << endl;
        string input = trim(inputData("", new char[12]{ "1234567890 " }, 11)) + ' ';
        unsigned int numberOfGrades;
        string* words = split(&input, false, &numberOfGrades);
        for (unsigned int i2 = 0; i2 < numberOfGrades; i2++) {
            unsigned int grade = stoi(words[i2]);
            grades_sum += grade;
            entry->grades[(lessons)i].push_back(grade);
        }
        grades_count += numberOfGrades;
    }
    //TODO Add limitations to prevent crashes
}

void write_entries(student_entry* entries , unsigned int size) {
    ofstream file("C:\\Users\\kloud\\Downloads\\entries.bin", ios::out | ios::binary);
    file.write(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        // determine the size of the string
        string::size_type sz = entries[i].fio.size();
        // write string size
        file.write(reinterpret_cast<char*>(&sz), sizeof(string::size_type));
        // and actual string
        file.write(entries[i].fio.data(), sz);

        file.write(reinterpret_cast<char*>(&entries[i].year_of_birth), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(&entries[i].group), sizeof(unsigned int));

        file.write(reinterpret_cast<char*>(&lessons_size), sizeof(unsigned int));
        for (unsigned int e = 0; e < lessons_size; e++) {
            unsigned int vector_size = entries[i].grades[(lessons)e].size();
            file.write(reinterpret_cast<char*>(&vector_size), sizeof(unsigned int));
            for (unsigned int m = 0; m < vector_size; m++) {
                file.write(reinterpret_cast<char*>(&entries[i].grades[(lessons)e].at(m)), sizeof(unsigned int));
            }
        }
        file.write(reinterpret_cast<char*>(&entries[i].grades_average), sizeof(unsigned int));
    }
    file.flush();
    file.close();
}

//student_entry* read_entries() {
//
//}

int main()
{
    SetConsoleOutputCP(65001);

    student_entry* entries = new student_entry[1];
    
    inputEntry(&entries[0]);
    write_entries(entries, 1);

    while (true) {
        switch (displaySelection(new string[5]{ "1.Создать файл", "2.Просмотреть", "3.Сортировать по среднему баллу", "4.Вывод по фильтру", "5.Выйти" }, 5)) {
            case 1:

                break;
        }
    }
}


