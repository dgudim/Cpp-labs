#include "Lab8.h"
#include "../genericFunctions.h"
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <map>
#include <fstream>
using namespace std;
#include <filesystem>
namespace fs = std::filesystem;

string workingDir = "../student_files/";
string currentFile = "";

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

    student_entry()
    {
        fio = "none";
        year_of_birth = 2000;
        group = 0;
        grades_average = 0;
    }
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
    entry->grades_average = grades_sum / (float)grades_count;
    //TODO Add limitations to prevent crashes
}

void write_entries(student_entry** entries , unsigned int size, string fileName) {
    ofstream file(workingDir + fileName, ios::out | ios::binary);
    file.write(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    file.write(reinterpret_cast<char*>(&lessons_size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        // determine the size of the string
        unsigned int fio_length = (*entries[i]).fio.length();
        // write string size
        file.write(reinterpret_cast<char*>(&fio_length), sizeof(unsigned int));
        // and actual string
        file.write((*entries[i]).fio.data(), fio_length);

        file.write(reinterpret_cast<char*>(&(*entries[i]).year_of_birth), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(&(*entries[i]).group), sizeof(unsigned int));
        
        for (unsigned int e = 0; e < lessons_size; e++) {
            unsigned int vector_size = (*entries[i]).grades[(lessons)e].size();
            file.write(reinterpret_cast<char*>(&vector_size), sizeof(unsigned int));
            for (unsigned int m = 0; m < vector_size; m++) {
                file.write(reinterpret_cast<char*>(&(*entries[i]).grades[(lessons)e].at(m)), sizeof(unsigned int));
            }
        }
        file.write(reinterpret_cast<char*>(&(*entries[i]).grades_average), sizeof(float));
    }
    file.flush();
    file.close();
}

student_entry* read_entries(string fileName, unsigned int *size_out) {
    unsigned int size = 0;
    student_entry* students;
    ifstream file(workingDir + fileName, ios::binary);
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(unsigned int))) {
        file.close();
        coutWithColor(6, "Файл пустой\n");
        currentFile = fileName;
        *size_out = 0;
        return NULL;
    }
    *size_out = size;
    students = new student_entry[size];
    unsigned int lessons_count = 0;
    file.read(reinterpret_cast<char*>(&lessons_count), sizeof(unsigned int));

    for (unsigned int i = 0; i < size; i++) {
        student_entry student;

        unsigned int fio_length = 0;
        file.read(reinterpret_cast<char*>(&fio_length), sizeof(unsigned int));
        student.fio.resize(fio_length);
        file.read(&student.fio[0], fio_length);
        file.read(reinterpret_cast<char*>(&student.year_of_birth), sizeof(unsigned int));
        file.read(reinterpret_cast<char*>(&student.group), sizeof(unsigned int));

        for (unsigned int e = 0; e < lessons_count; e++) {
            unsigned int vector_size = 0;
            file.read(reinterpret_cast<char*>(&vector_size), sizeof(unsigned int));
            for (unsigned int v = 0; v < vector_size; v++) {
                unsigned int grade = 0;
                file.read(reinterpret_cast<char*>(&grade), sizeof(unsigned int));
                student.grades[(lessons)e].push_back(grade);
            }
        }

        file.read(reinterpret_cast<char*>(&student.grades_average), sizeof(float));

        students[i] = student;
    }
    file.close();

    currentFile = fileName;
    coutWithColor(10, "Успешно загрузил " + to_string(size) + " записей\n");

    return students;
}

unsigned int findMaxNameLength(student_entry* students, unsigned int size) {
    unsigned int maxLength = 0;
    for (unsigned int i = 0; i < size; i++) {
        maxLength = max(maxLength, students[i].fio.length());
    }
    return maxLength;
}

void printEntries(student_entry* students, unsigned int size) {
    coutWithColor(3, "Количество студентов: " + to_string(size) + "\n");
    if (size == 0) {
        return;
    }
    unsigned int maxNameLength = max(findMaxNameLength(students, size), (unsigned int)7);
    //"Студент".length() is 14 for some reason, investigate
    cout << addSpaces("Студент|", maxNameLength) << "Год рождения|" << "Номер группы|" << "Средний балл" << endl;
    for (unsigned int i = 0; i < size; i++) {

        cout << addSpaces(students[i].fio, maxNameLength) << "|" << addSpaces(to_string(students[i].year_of_birth), 12) << "|";
        cout << addSpaces(to_string(students[i].group), 12) << "|" << students[i].grades_average << endl;

        /*for (unsigned int g = 0; g < lessons_size; g++) {
            unsigned int grades_size = students[i].grades[(lessons)g].size();
            cout << "\nОтметки по " << lessons_map_case[g] << ": " << endl;
            for (unsigned int v = 0; v < grades_size; v++) {
                cout << students[i].grades[(lessons)g].at(v);
            }
        }*/
    }
}

void createFile() {
    bool exit;
    string fileName;
    while(true){
        exit = true;

        fileName = inputData("Введите название файла: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64);

        ifstream file_read(workingDir + fileName + ".dat", ios::out | ios::binary);

        if (file_read.is_open()) {
            unsigned int success_bytes = 0;
            if (file_read.read(reinterpret_cast<char*>(&success_bytes), sizeof(unsigned int))) {
                string input = displayWarningWithInput(6, "Файл существует и в нем есть данные, перезаписать?\n");
                if (!(input == "yes" || input == "y" || input == "1")) {
                    exit = false;
                }
            }
            file_read.close();
        }
        if (exit) {
            ofstream file(workingDir + fileName + ".dat", ios::out | ios::binary);
            file.close();
            coutWithColor(10, "Файл был успешно создан\n");
            break;
        }
    }
}

void loadFromFile(student_entry** entries, unsigned int* size) {
    unsigned int files = 0;
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        files++;
    }
    string* file_names = new string[files];
    unsigned int i = 0;
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        file_names[i] = entry.path().filename().u8string();
        i++;
    }
    int file_chosen = displaySelection(file_names, files);
    student_entry* read_entries_ = read_entries(file_names[file_chosen - 1], size);
    delete[] file_names;
    delete[] *entries;
    *entries = read_entries_;
}

void listFiles() {
    coutWithColor(11, "Список файлов:\n");
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        cout << entry.path().filename().u8string() << endl;
    }
}

void addEntries(student_entry** entries, unsigned int *size) {
    unsigned int entries_to_add = (unsigned int)inputData("Сколько записей добавить?\n");
    unsigned int newSize = entries_to_add + *size;
    student_entry* newEntries = new student_entry[newSize];
    for (unsigned int i = 0; i < *size; i++) {
        newEntries[i] = *entries[i];
    }
    for (unsigned int i = *size; i < newSize; i++) {
        student_entry entry;
        inputEntry(&entry);
        newEntries[i] = entry;
    }
    *size = newSize;
    delete[] *entries;
    *entries = newEntries;
}

void edit(student_entry** entries, unsigned int *size) {
    coutWithColor(11, "Записи:\n");
    printEntries(*entries, *size);
    switch (displaySelection(new string[5]{ "1.Добавить записи", "2.Просмотреть записи", "3.Удалить запись", "4.Редактировать запись", "5.Сортировать по среднему баллу" }, 5)) {
    case 1:
        addEntries(entries, size);
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    case 4:

        break;
    case 5:
        
        break;
    }
    write_entries(entries, *size, currentFile);
}

int main()
{
    SetConsoleOutputCP(65001);
    listFiles();

    student_entry* entries = new student_entry[0];
    unsigned int entries_size;

    while (true) {
        coutWithColor(11, "\n_________Меню (Выбор стрелками и Enter)_________\n");
        coutWithColor(3, "Текущий файл: " + currentFile + "\n\n");
        switch (displaySelection(new string[5]{ "1.Создать файл", "2.Открыть файл", "3.Просмотреть файлы", "4.Редактировать/просмотреть текущий файл", "5.Выйти" }, 5)) {
            case 1:
                createFile();
                break;
            case 2:
                loadFromFile(&entries, &entries_size);
                break;
            case 3:
                listFiles();
                break;
            case 4:
                if (currentFile != "") {
                    edit(&entries, &entries_size);
                }
                else {
                    coutWithColor(6, "Файл не выбран, выберите файл или создайте новый\n");
                }
                break;
            case 5:
                string input = displayWarningWithInput(6, "Вы уверены, что хотите выйти?\n");
                if (input == "yes" || input == "y" || input == "1") {
                    exit(-15);
                }
                break;
        }
    }
}


