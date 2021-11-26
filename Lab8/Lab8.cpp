#include "Lab8.h"
#include "../genericFunctions.h"

using namespace std;
namespace fs = std::filesystem;

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
        fio = "none";
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
        coutWithColor(11, "\n_________Меню (Выбор стрелками и Enter)_________\n");
        coutWithColor(3, "Текущий файл: " + currentFile + "\n\n");
        switch (displaySelection(new string[4]{ "1.Создать файл", "2.Открыть файл", "3.Редактировать/просмотреть текущий файл", "4.Выйти" }, 4)) {
            case 1:
                createFile();
                break;
            case 2:
                loadFromFile(&entries);
                break;
            case 3:
                reset();
                if (currentFile != "") {
                    edit(&entries);
                }
                else {
                    coutWithColor(6, "Файл не выбран, выберите файл или создайте новый\n");
                }
                break;
            case 4:
                string input = displayWarningWithInput(6, "Вы уверены, что хотите выйти?\n");
                if (input == "yes" || input == "y" || input == "1") {
                    exit(-15);
                }
                break;
        }
    }
}

void reset() {
    system("CLS");
}

void inputEntry(student_entry* entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53, regex(".*[.].[.]."));
    entry->year_of_birth = stoi(inputData("Введите год рождения: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9]")));
    entry->group = stoi(inputData("Введите номер группы: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9][0-9][0-9]")));
    unsigned int grades_sum = 0;
    unsigned int grades_count = 0;
    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел: " << flush;
        string input = trim(inputData("", new char[12]{ "1234567890 " }, 11)) + ' ';
        if (input.length() != 1) {
            unsigned int numberOfGrades;
            string* words = split(&input, false, &numberOfGrades);
            for (unsigned int i2 = 0; i2 < numberOfGrades; i2++) {
                unsigned int grade = stoi(words[i2]);
                grades_sum += grade;
                entry->grades[(lessons)i].push_back(grade);
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
    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Введите отметки по " << lessons_map_case[i] << " через пробел: " << flush;

        unsigned int previous_grades_len = entry->grades[(lessons)i].size();
        string previous_grades;
        for (unsigned int gr = 0; gr < previous_grades_len; gr++) {
            previous_grades.append(to_string(entry->grades[(lessons)i].at(gr)) + " ");
        }

        string input = trim(inputData("", new char[12]{ "1234567890 " }, 11, regex(".*"), previous_grades)) + ' ';
        if (input.length() != 1) {
            unsigned int numberOfGrades;
            string* words = split(&input, false, &numberOfGrades);
            for (unsigned int i2 = 0; i2 < numberOfGrades; i2++) {
                unsigned int grade = stoi(words[i2]);
                grades_sum += grade;
                entry->grades[(lessons)i].push_back(grade);
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
    reset();
    coutWithColor(10, "Сохранил изменения\n");
    file.flush();
    file.close();
}

void read_entries(vector<student_entry>* entries, string fileName) {
    unsigned int size = 0;
    entries->clear();
    ifstream file(workingDir + fileName, ios::binary);
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(unsigned int))) {
        file.close();
        reset();
        coutWithColor(6, "Файл пустой\n");
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
    reset();
    coutWithColor(10, "Успешно загрузил " + to_string(size) + " записей\n");
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
    coutWithColor(11, "Записи:");
    coutWithColor(3, "\nКоличество студентов: " + to_string(size) + "\n");
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

void alphabeticSort(vector<student_entry>* entries) {
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
            if (entries->at(l).fio > entries->at(l + 1).fio)
            {
                iter_swap(entries->begin() + l, entries->begin() + l + 1);
                swap = true;
            }
        }
    }
}

void gradeSort(vector<student_entry>* entries) {
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
            if (entries->at(l).grades_average < entries->at(l + 1).grades_average)
            {
                iter_swap(entries->begin() + l, entries->begin() + l + 1);
                swap = true;
            }
        }
    }
}

void createFile() {
    bool exit;
    string fileName;
    while (true) {
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
            reset();
            coutWithColor(10, "Файл был успешно создан\n");
            break;
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

void listFiles() {
    coutWithColor(11, "Список файлов:\n");
    for (const auto& entry : fs::directory_iterator(workingDir)) {
        cout << entry.path().filename().u8string() << endl;
    }
}

void viewEntries(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = entries->at(i).fio;
    }
    coutWithColor(14, "Выберите студентов (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* selected = displayMultiSelection(selection, size);
    for (unsigned int i = 0; i < size; i++) {
        if (selected[i]) {
            printEntry(&(entries->at(i)));
        }
    }
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
    coutWithColor(14, "Выберите студентов (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* selected = displayMultiSelection(selection, size);

    string input = displayWarningWithInput(6, "Вы уверены, что хотите удалить?\n");
    if (input == "yes" || input == "y" || input == "1") {
        for (unsigned int i = 0; i < size; i++) {
            entries->at(i).valid = !selected[i];
        }
        entries->erase(remove_if(entries->begin(), entries->end(), isInvalid), entries->end());
    }
}

void addEntries(vector<student_entry>* entries) {
    unsigned int entries_to_add = (unsigned int)inputData("Сколько записей добавить?\n");
    unsigned int newSize = entries_to_add + entries->size();
    for (unsigned int i = 0; i < entries_to_add; i++) {
        student_entry entry;
        inputEntry(&entry);
        entries->push_back(entry);
    }
}

void editEntries(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = entries->at(i).fio;
    }
    coutWithColor(14, "Выберите студента\n");
    editEntry(&(entries->at(displaySelection(selection, size) - 1)));
}

void edit(vector<student_entry>* entries) {
    bool exit = false;
    bool save = false;
    while (true) {
        printSummary(entries);
        bool save = true;
        switch (displaySelection(new string[7]{ "1.Добавить записи", "2.Просмотреть записи", "3.Удалить записи", "4.Редактировать запись", "5.Сортировать по алфавиту", "6.Сортировать по среднему баллу", "7.Назад" }, 7)) {
        case 1:
            addEntries(entries);
            break;
        case 2:
            reset();
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
            alphabeticSort(entries);
            break;
        case 6:
            gradeSort(entries);
            break;
        case 7:
            exit = true;
            break;
        }
        if (exit) {
            reset();
            break;
        }
        if (save) {
            write_entries(entries, currentFile);
            save = false;
        }
    }
}