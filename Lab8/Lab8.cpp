#include "Lab8.h"
#include "../genericFunctions.h"

using namespace std;
namespace fs = std::filesystem;

#pragma execution_character_set( "utf-8" )

enum lessons {
    PHYSICS = 0,
    MATH = 1,
    INFORMATICS = 2,
    CHEMISTRY = 3
};

string workingDir = "../student_files/";
string currentFile = "";

unsigned int lessons_size = 4;
const char* lessons_map[] = { "Физика", "Математика", "Информатика", "Химия" };
const char* lessons_map_case[] = { "физике", "математике", "информатике", "химии" };

struct student_entry {

    student_entry()
    {
        fio = "None.N.N";
        year_of_birth = 2000;
        group = 0;
        grades_average = 0;
        valid = true;
    }
    string fio;
    unsigned int year_of_birth;
    unsigned int group;
    map<lessons, vector<unsigned int>> grades;
    float grades_average;
    bool valid;
};

int main()
{
    SetConsoleOutputCP(65001);

    vector<student_entry> entries;

    while (true) {
        listFiles();
        coutWithColor(colors::CYAN, "\nМеню (Выбор стрелками и Enter)\n");
        coutWithColor(colors::LIGHTER_BLUE, "Текущий файл: " + currentFile + "\n\n");
        switch (displaySelection(new string[5]{ "1.Открыть файл", "2.Создать файл", "3.Удалить файлы", "4.Редактировать/просмотреть текущий файл", "5.Выйти" }, 5)) {
            case 1:
                loadFromFile(&entries);
                break;
            case 2:
                createFile();
                break;
            case 3:
                deleteFiles();
                break;
            case 4:
                clearScreen();
                if (currentFile != "") {
                    edit(&entries);
                }
                else {
                    coutWithColor(colors::YELLOW , "Файл не открыт, откройте файл или создайте новый\n");
                }
                break;
            case 5:
                string input = displayWarningWithInput(6, "Вы уверены, что хотите выйти?\n");
                if (input == "yes" || input == "y" || input == "1") {
                    exit(-15);
                }
                else {
                    clearScreen();
                }
                break;
        }
    }
}

void edit(vector<student_entry>* entries) {
    bool exit = false;
    bool save = false;
    while (true) {
        printSummary(entries);
        bool save = true;
        switch (displaySelection(new string[6]{ "1.Добавить записи", "2.Просмотреть записи", "3.Удалить записи", "4.Редактировать запись", "5.Сортировать по выбранному полю", "6.Назад" }, 6)) {
        case 1:
            save = addEntries(entries);
            break;
        case 2:
            viewEntries(entries);
            save = false;
            break;
        case 3:
            deleteEntries(entries);
            break;
        case 4:
            editEntries(entries);
            break;
        case 5:
            sort(entries);
            save = false;
            break;
        case 6:
            exit = true;
            break;
        }
        if (exit) {
            clearScreen();
            break;
        }
        if (save) {
            write_entries(entries, currentFile);
            save = false;
        }
    }
}

void sort(vector<student_entry>* entries) {
    coutWithColor(colors::LIGHT_YELLOW, "Сортировать по\n");
    bool sort = true;
    SortFunction sortFunction = NULL;
    switch (displaySelection(new string[5]{ "1.Алфавиту", "2.Году рождения", "3.Номеру группы", "4.Среднему баллу", "5.Отмена" }, 5)) {
    case 1:
        sortFunction = fio_compare;
        break;
    case 2:
        sortFunction = year_compare;
        break;
    case 3:
        sortFunction = group_compare;
        break;
    case 4:
        sortFunction = avg_grade_compare;
        break;
    case 5:
        sort = false;
        break;
    }
    if (sort) {
        vector<student_entry> prev_entries = *entries;
        entrySort(entries, sortFunction);
        printSummary(entries);
        coutWithColor(colors::LIGHT_YELLOW, "Сохранить изменения?\n");
        switch(displaySelection(new string[3]{ "1.Да", "2.Да, в другой файл", "3.Нет"}, 3)) {
        case 1:
            write_entries(entries, currentFile);
            break;
        case 2:
            write_entries(entries, createFile());
            break;
        case 3:
            *entries = prev_entries;
            break;
        }
    }
}

void clearScreen() {
    system("CLS");
}

void inputEntry(student_entry* entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53, regex(".*[.].[.]."));
    entry->year_of_birth = stoi(inputData("Введите год рождения: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9]")));
    entry->group = stoi(inputData("Введите номер группы: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9][0-9][0-9]")));
    unsigned int grades_sum = 0;
    unsigned int grades_count = 0;
    string failureBuffer = "";
    string lastInput = "";
    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел: " << flush;
        string input = trim(inputData("", new char[12]{ "1234567890 " }, 11, failureBuffer)) + ' ';
        lastInput = input;
        failureBuffer = "";
        if (input.length() != 1) {
            unsigned int numberOfGrades;
            unsigned int numberOfOutOfBoundsGrades = 0;
            string* words = split(&input, false, &numberOfGrades);
            for (unsigned int i2 = 0; i2 < numberOfGrades; i2++) {
                double grade_signed = stod(words[i2]);
                numberOfOutOfBoundsGrades += (grade_signed < 0 || grade_signed > 10);
                unsigned int grade = (unsigned int)min(max(grade_signed, 0.0), 10.0);
                grades_sum += grade;
                entry->grades[(lessons)i].push_back(grade);
            }
            delete[] words;
            if (numberOfOutOfBoundsGrades > 0) {
                coutWithColor(colors::YELLOW, "Некоторые введенные отметки(" +to_string(numberOfOutOfBoundsGrades)+ ") выходят за границы допустимого диапазона(0 - 10)\n");
                failureBuffer = lastInput;
                i--;
            }
            grades_count += numberOfGrades;
        }
    }
    if (grades_count == 0) {
        entry->grades_average = 0;
    }
    else {
        entry->grades_average = grades_sum / (float)grades_count;
    }
}

void editEntry(student_entry* entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53, regex(".*[.].[.]."), entry->fio);
    entry->year_of_birth = stoi(inputData("Введите год рождения: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9]"), to_string(entry->year_of_birth)));
    entry->group = stoi(inputData("Введите номер группы: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9][0-9][0-9]"), to_string(entry->group)));
    unsigned int grades_sum = 0;
    unsigned int grades_count = 0;
    string failureBuffer = "";
    string lastInput = "";
    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел: " << flush;
        string input;

        if (failureBuffer == "") {
            unsigned int previous_grades_len = entry->grades[(lessons)i].size();
            string previous_grades;
            for (unsigned int gr = 0; gr < previous_grades_len; gr++) {
                previous_grades.append(to_string(entry->grades[(lessons)i].at(gr)) + " ");
            }
            input = trim(inputData("", new char[12]{ "1234567890 " }, 11, previous_grades)) + ' ';
        }
        else {
            input = trim(inputData("", new char[12]{ "1234567890 " }, 11, failureBuffer)) + ' ';
            failureBuffer = "";
        }

        lastInput = input;

        if (input.length() != 1) {
            unsigned int numberOfGrades;
            unsigned int numberOfOutOfBoundsGrades = 0;
            string* words = split(&input, false, &numberOfGrades);
            for (unsigned int i2 = 0; i2 < numberOfGrades; i2++) {
                double grade_signed = stod(words[i2]);
                numberOfOutOfBoundsGrades += (grade_signed < 0 || grade_signed > 10);
                unsigned int grade = (unsigned int)min(max(grade_signed, 0.0), 10.0);
                grades_sum += grade;
                entry->grades[(lessons)i].push_back(grade);
            }
            delete[] words;
            if (numberOfOutOfBoundsGrades > 0) {
                coutWithColor(colors::YELLOW, "Некоторые введенные отметки(" + to_string(numberOfOutOfBoundsGrades) + ") выходят за границы допустимого диапазона(0 - 10)\n");
                failureBuffer = lastInput;
                i--;
            }
            grades_count += numberOfGrades;
        }
    }
    if (grades_count == 0) {
        entry->grades_average = 0;
    }
    else {
        entry->grades_average = grades_sum / (float)grades_count;
    }
}

void write_entries(vector<student_entry>* entries, string fileName) {
    ofstream file(workingDir + fileName, ios::out | ios::binary);
    unsigned int size = entries->size();
    file.write(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    file.write(reinterpret_cast<char*>(&lessons_size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        // determine the size of the string
        unsigned int fio_length = entries->at(i).fio.length();
        // write string size
        file.write(reinterpret_cast<char*>(&fio_length), sizeof(unsigned int));
        // and actual string
        file.write(entries->at(i).fio.data(), fio_length);

        file.write(reinterpret_cast<char*>(&(entries->at(i).year_of_birth)), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(&(entries->at(i).group)), sizeof(unsigned int));

        for (unsigned int e = 0; e < lessons_size; e++) {
            unsigned int vector_size = entries->at(i).grades[(lessons)e].size();
            file.write(reinterpret_cast<char*>(&vector_size), sizeof(unsigned int));
            for (unsigned int m = 0; m < vector_size; m++) {
                file.write(reinterpret_cast<char*>(&(entries->at(i).grades[(lessons)e].at(m))), sizeof(unsigned int));
            }
        }
        file.write(reinterpret_cast<char*>(&(entries->at(i).grades_average)), sizeof(float));
    }
    clearScreen();
    coutWithColor(colors::LIGHT_GREEN, "Сохранил изменения\n");
    file.flush();
    file.close();
}

void read_entries(vector<student_entry>* entries, string fileName) {
    unsigned int size = 0;
    entries->clear();
    ifstream file(workingDir + fileName, ios::in | ios::binary);
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(unsigned int))) {
        file.close();
        clearScreen();
        coutWithColor(colors::YELLOW, "Файл пустой\n");
        currentFile = fileName;
        return;
    }
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

        entries->push_back(student);
    }
    file.close();

    currentFile = fileName;
    clearScreen();
    coutWithColor(colors::LIGHT_GREEN, "Успешно загрузил " + to_string(size) + " записей\n");
}

unsigned int findMaxNameLength(vector<student_entry>* entries, unsigned int size) {
    unsigned int maxLength = 0;
    for (unsigned int i = 0; i < size; i++) {
        maxLength = max(maxLength, entries->at(i).fio.length());
    }
    return maxLength;
}

void printEntry(student_entry* entry) {
    cout << "\n";
    setConsoleColor(10);
    cout << "Ф.И.О: " << entry->fio << endl;
    setConsoleColor(7);
    cout << "Год рождения: " << entry->year_of_birth << endl;
    cout << "Номер группы: " << entry->group << endl;
    cout << "Отметки: " << endl;
    for (unsigned int g = 0; g < lessons_size; g++) {
        unsigned int grades_size = entry->grades[(lessons)g].size();
        cout << lessons_map[g] << ": ";
        for (unsigned int v = 0; v < grades_size; v++) {
            cout << entry->grades[(lessons)g].at(v) << " ";
        }
        cout << endl;
    }
    cout << "Средний балл: " << entry->grades_average << endl;
}

void printSummary(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    coutWithColor(colors::LIGHT_YELLOW, "\nТекущий файл: " + currentFile);
    coutWithColor(colors::CYAN, "\nЗаписи:");
    coutWithColor(colors::LIGHTER_BLUE, "\nКоличество студентов: " + to_string(size) + "\n");
    if (size == 0) {
        return;
    }
    unsigned int maxNameLength = max(findMaxNameLength(entries, size), (unsigned int)7);
    cout << "Студент" << addSpaces("", maxNameLength - 7) << "|" << "Год рождения|" << "Номер группы|" << "Средний балл" << endl;
    for (unsigned int i = 0; i < size; i++) {
        cout << addSpaces(entries->at(i).fio, maxNameLength) << "|" << addSpaces(to_string(entries->at(i).year_of_birth), 12) << "|";
        cout << addSpaces(to_string(entries->at(i).group), 12) << "|" << entries->at(i).grades_average << endl;
    }
    cout << endl;
}

bool fio_compare(student_entry entry1, student_entry entry2) {
    return entry1.fio > entry2.fio;
}

bool avg_grade_compare(student_entry entry1, student_entry entry2) {
    return entry1.grades_average < entry2.grades_average;
}

bool group_compare(student_entry entry1, student_entry entry2) {
    return entry1.group < entry2.group;
}

bool year_compare(student_entry entry1, student_entry entry2) {
    return entry1.year_of_birth < entry2.year_of_birth;
}

void entrySort(vector<student_entry>* entries, SortFunction sortFunction) {
    int j = 0;
    bool swap = true;
    string temp;
    unsigned int size = entries->size();
    while (swap)
    {
        swap = false;
        j++;
        for (unsigned int l = 0; l < size - j; l++)
        {
            if (sortFunction(entries->at(l), entries->at(l + 1)))
            {
                iter_swap(entries->begin() + l, entries->begin() + l + 1);
                swap = true;
            }
        }
    }
}

string createFile() {
    bool exit;
    string fileName;
    while (true) {
        exit = true;

        fileName = inputData("Введите название файла: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64);

        ifstream file_read(workingDir + fileName + ".dat", ios::in | ios::binary);

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
            clearScreen();
            coutWithColor(colors::LIGHT_GREEN, "Файл был успешно создан\n");
            return fileName + ".dat";
        }
    }
}

void loadFromFile(vector<student_entry>* entries) {
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
    read_entries(entries, file_names[file_chosen - 1]);
    delete[] file_names;
}

void deleteFiles() {
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
    coutWithColor(colors::LIGHT_YELLOW, "Выберите файлы (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* files_chosen = displayMultiSelection(file_names, files);
    unsigned int deleted_files = 0;
    unsigned int files_to_delete = 0;

    for (unsigned int i = 0; i < files; i++) {
        if (files_chosen[i]) {
            files_to_delete++;
        }
    }

    if (files_to_delete > 0) {
        string input = displayWarningWithInput(6, "Вы уверены, что хотите удалить?\n");
        if (input == "yes" || input == "y" || input == "1") {
            for (unsigned int i = 0; i < files; i++) {
                if (files_chosen[i]) {
                    if (file_names[i] == currentFile) {
                        currentFile = "";
                    }
                    deleted_files += (remove((workingDir + file_names[i]).c_str()) == 0 ? 1 : 0);
                }
            }
            clearScreen();
            coutWithColor(colors::LIGHT_GREEN, "Удалил " + to_string(deleted_files) + " файлов\n");
        }
        else {
            clearScreen();
        }
    }
    else {
        clearScreen();
    }
    delete[] file_names;
    delete[] files_chosen;
}

unsigned int getStats(string path) {
    ifstream file_read(workingDir + path, ios::in | ios::binary);
    if (file_read.is_open()) {
        unsigned int size = 0;
        if (file_read.read(reinterpret_cast<char*>(&size), sizeof(unsigned int))) {
            return size;
        }
        file_read.close();
    }
    return 0;
}

void listFiles() {
    coutWithColor(colors::CYAN, "Список файлов:\n");
    unsigned int maxLen = 0;
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        maxLen = max(entry.path().filename().u8string().length(), maxLen);
    }
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        string name = entry.path().filename().u8string();
        bool current = name == currentFile;
        coutWithColor(current ? colors::LIGHT_GREEN : colors::DEFAULT, addSpaces(name, maxLen + 1));
        cout << "(" << getStats(name) << " записей)" << (current ? " -- текущий" : "") << endl;
    }
}

void viewEntries(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = entries->at(i).fio;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите студентов (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* selected = displayMultiSelection(selection, size);
    clearScreen();
    for (unsigned int i = 0; i < size; i++) {
        if (selected[i]) {
            printEntry(&(entries->at(i)));
        }
    }
    cout << endl;
    delete[] selected;
}

bool isInvalid(student_entry entry) {
    return !entry.valid;
}

void deleteEntries(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = entries->at(i).fio;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите студентов (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* selected = displayMultiSelection(selection, size);

    string input = displayWarningWithInput(6, "Вы уверены, что хотите удалить?\n");
    if (input == "yes" || input == "y" || input == "1") {
        for (unsigned int i = 0; i < size; i++) {
            entries->at(i).valid = !selected[i];
        }
        entries->erase(remove_if(entries->begin(), entries->end(), isInvalid), entries->end());
    }

    delete[] selected;
}

bool addEntries(vector<student_entry>* entries) {
    unsigned int entries_to_add = (unsigned int)max(inputData("Сколько записей добавить?\n"), 0.0);
    
    for (unsigned int i = 0; i < entries_to_add; i++) {
        student_entry entry;
        inputEntry(&entry);
        entries->push_back(entry);
    }
    return entries_to_add > 0;
}

void editEntries(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = entries->at(i).fio;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите студента\n");
    editEntry(&(entries->at(displaySelection(selection, size) - 1)));
}