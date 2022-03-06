#include "Lab8.h"
#include "genericFunctions.h"

using namespace std;
namespace fs = std::filesystem;

#pragma execution_character_set( "utf-8" )

string workingDir = "../student_files/";
string userInfo_fileName = "users.dat";
string currentFile = "";

unsigned int lessons_size = 5;
const char* lessons_map[] = { "Физика", "Математика", "Информатика", "Химия", "Логика"};
const char* lessons_map_case[] = { "физике", "математике", "информатике", "химии", "логике"};

unsigned int tests_size = 5;
const char* tests_map[] = { "Философия", "История", "Русс.яз", "Бел.яз", "Физкультура" };
const char* tests_map_case[] = { "философии", "истории", "русс.язу", "бел.язу", "физкультуре" };

struct student_entry {

    student_entry()
    {
        fio = "None.N.N";
        group = 0;
        debts = 0;
        valid = true;
    }
    string fio;
    unsigned int group;
    vector<unsigned int> exam_grades;
    vector<char> test_results;
    unsigned int debts;
    bool valid;
};

struct user_entry {
    user_entry() {
        valid = true;
    }
    bool valid;
    char access_level;
    size_t password;
    string login;
};

user_entry currentUser;

int main()
{
    vector<user_entry> users;
    vector<student_entry> entries;

    bool exit;

    while (true) {

        login(&users);
        exit = false;

        while (!exit) {
            coutWithColor(colors::LIGHTER_BLUE, "Текущий пользователь: " + currentUser.login + ", роль: " + ((currentUser.access_level == 1) ? "администратор\n" : "пользователь\n"));
            if (currentUser.access_level == 1) {
                exit = displayAdminMenu(&users, &entries);
            }
            else {
                exit = displayUserMenu(&users, &entries);
            }
        }
    }
}

bool displayUserMenu(vector<user_entry>* users, vector<student_entry>* entries) {
    listFiles();
    coutWithColor(colors::CYAN, "\nМеню (Выбор стрелками и Enter)\n");
    coutWithColor(colors::LIGHTER_BLUE, "Текущий файл: " + currentFile + "\n\n");
    switch (displaySelection(new string[3]{
        "1.Открыть файл",
        "2.Просмотреть текущий файл, поиск по файлу",
        "3.Выйти" }, 3)) {
    case 1:
        loadFromFile(entries);
        break;
    case 2:
        clearScreen();
        if (currentFile != "") {
            edit_minimal(entries);
        }
        else {
            coutWithColor(colors::YELLOW, "Файл не открыт, откройте файл или создайте новый\n");
        }
        break;
    case 3:
        string input = displayWarningWithInput(colors::YELLOW, "Вы уверены, что хотите выйти?\n");
        if (input == "yes" || input == "y" || input == "1") {
            clearScreen();
            return true;
        }
        clearScreen();
        break;
    }
    return false;
}

bool displayAdminMenu(vector<user_entry> *users, vector<student_entry> *entries) {

    listUsers(users);
    listFiles();
    coutWithColor(colors::CYAN, "\nМеню (Выбор стрелками и Enter)\n");
    coutWithColor(colors::LIGHTER_BLUE, "Текущий файл: " + currentFile + "\n\n");
    switch (displaySelection(new string[8]{
        "1.Открыть файл",
        "2.Создать файл",
        "3.Удалить файлы",
        "4.Редактировать/просмотреть текущий файл",
        "5.Добавить нового пользователя",
        "6.Удалить пользователей",
        "7.Редактировать пользователя",
        "8.Выйти" }, 8)) {
    case 1:
        loadFromFile(entries);
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
            edit(entries);
        }
        else {
            coutWithColor(colors::YELLOW, "Файл не открыт, откройте файл или создайте новый\n");
        }
        break;
    case 5:
        add_user(users);
        break;
    case 6:
        deleteUsers(users);
        break;
    case 7:
        editUsers(users);
        break;
    case 8:
        string input = displayWarningWithInput(colors::YELLOW, "Вы уверены, что хотите выйти?\n");
        if (input == "yes" || input == "y" || input == "1") {
            clearScreen();
            return true;
        }
        clearScreen();
        break;
    }
    return false;
}

void edit(vector<student_entry>* entries) {
    bool exit = false;
    bool save = false;
    while (true) {
        printSummary(entries);
        bool save = true;
        switch (displaySelection(new string[8]{ 
            "1.Добавить записи", 
            "2.Просмотреть записи", 
            "3.Удалить записи", 
            "4.Редактировать запись", 
            "5.Сортировать по выбранному полю", 
            "6.Поиск по выбранному полю", 
            "7.Информация по группе(личное задание)",
            "8.Назад" }, 8)) {
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
            search(entries);
            save = false;
            break;
        case 7:
            printGroupSummary(entries);
            save = false;
            break;
        case 8:
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

void edit_minimal(vector<student_entry>* entries) {
    bool exit = false;
    while (!exit) {
        printSummary(entries);
        switch (displaySelection(new string[5]{ 
            "1.Просмотреть записи", 
            "2.Сортировать по выбранному полю", 
            "3.Поиск по выбранному полю",
            "4.Информация по группе(личное задание)"
            "5.Назад" }, 5)) {
        case 1:
            viewEntries(entries);
            break;
        case 2:
            sort(entries);
            break;
        case 3:
            search(entries);
            break;
        case 4:
            printGroupSummary(entries);
            break;
        case 5:
            exit = true;
            break;
        }
    }
    clearScreen();
}

void sort(vector<student_entry>* entries) {
    coutWithColor(colors::LIGHT_YELLOW, "Сортировать по\n");
    bool sort = true;
    SortFunction sortFunction = NULL;
    switch (displaySelection(new string[5]{ "1.Алфавиту", "2.Номеру группы", "3.Количеству задолженностей", "4.Отмена" }, 5)) {
    case 1:
        sortFunction = fio_compare;
        break;
    case 2:
        sortFunction = group_compare;
        break;
    case 3:
        sortFunction = debt_compare;
        break;
    case 4:
        sort = false;
        break;
    }
    if (sort) {
        vector<student_entry> prev_entries = *entries;
        entrySort(entries, sortFunction);
        printSummary(entries);
        coutWithColor(colors::LIGHT_YELLOW, "Сохранить изменения?\n");
        switch (displaySelection(new string[3]{ "1.Да", "2.Да, в другой файл", "3.Нет" }, 3)) {
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
    clearScreen();
}

void search(vector<student_entry>* entries) {
    string search = inputData("Поиск: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64);
    coutWithColor(colors::LIGHT_YELLOW, "Поиск по\n");
    bool sort = true;
    SearchFunction searchFunction = NULL;
    switch (displaySelection(new string[5]{ "1.Имени", "2.Номеру группы", "3.Количеству задолженностей", "4.Отмена" }, 5)) {
    case 1:
        searchFunction = fio_match;
        break;
    case 2:
        searchFunction = group_match;
        break;
    case 3:
        searchFunction = debt_match;
        break;
    case 4:
        sort = false;
        break;
    }
    if (sort) {
        vector<student_entry> prev_entries = *entries;
        entrySearch(entries, searchFunction, search);
        if (entries->size() == 0) {
            coutWithColor(colors::LIGHT_RED, "\nНичего не найдено, нажмите любую клавишу, чтобы продолжить\n");
            _getch();
            *entries = prev_entries;
            clearScreen();
            return;
        }
        coutWithColor(colors::LIGHT_YELLOW, "\nРезультаты поиска: \n");
        printSummary(entries);
        coutWithColor(colors::LIGHT_YELLOW, "Сохранить результат?\n");
        switch (displaySelection(new string[3]{ "1.Да, в текущий файл", "2.Да, в другой файл", "3.Нет" }, 3)) {
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
    clearScreen();
}

void inputEntry(student_entry* entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53, regex(".*[.].[.]."));
    entry->group = stoi(inputData("Введите номер группы: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9][0-9][0-9]")));

    for (unsigned int i = 0; i < lessons_size; i++) {
        entry->exam_grades.push_back(inputData("Введите отметку по экзамену по " + string(lessons_map_case[i]) + ": ", false));
    }
    for (unsigned int i = 0; i < tests_size; i++) {
        entry->test_results.push_back(inputData("Введите результат зачета по " + string(tests_map_case[i]) + " (больше 5и - это зачет, иначе незачет): ", false) > 5);
    }

    entry->debts = inputData("Введите количество задолженностей: ", false);
}

void editEntry(student_entry* entry) {
    entry->fio = inputData("Введите Ф.И.О: ", new char[54]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM." }, 53, regex(".*[.].[.]."), entry->fio, false);
    entry->group = stoi(inputData("Введите номер группы: ", new char[11]{ "1234567890" }, 10, regex("[1-9][0-9][0-9][0-9][0-9][0-9]"), to_string(entry->group), false));

    for (unsigned int i = 0; i < lessons_size; i++) {
        entry->exam_grades.push_back(inputData("Введите отметку по экзамену по " + string(lessons_map_case[i]) + ": ", false));
    }
    for (unsigned int i = 0; i < tests_size; i++) {
        entry->test_results.push_back(inputData("Введите результат зачета по " + string(tests_map_case[i]) + " (больше 5и - это зачет, иначе незачет): ", false) > 5);
    }

    entry->debts = inputData("Введите количество задолженностей: ", false);
}

void write_entries(vector<student_entry>* entries, string fileName) {
    ofstream file(workingDir + fileName, ios::out | ios::binary);
    unsigned int size = entries->size();
    file.write(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    file.write(reinterpret_cast<char*>(&lessons_size), sizeof(unsigned int));
    file.write(reinterpret_cast<char*>(&tests_size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        // determine the size of the string
        unsigned int fio_length = entries->at(i).fio.length();
        // write string size
        file.write(reinterpret_cast<char*>(&fio_length), sizeof(unsigned int));
        // and actual string
        file.write(entries->at(i).fio.data(), fio_length);

        file.write(reinterpret_cast<char*>(&(entries->at(i).group)), sizeof(unsigned int));

        for (unsigned int e = 0; e < lessons_size; e++) {
            file.write(reinterpret_cast<char*>(&(entries->at(i).exam_grades.at(e))), sizeof(unsigned int));
        }

        for (unsigned int e = 0; e < tests_size; e++) {
            file.write(reinterpret_cast<char*>(&(entries->at(i).test_results.at(e))), sizeof(char));
        }

        file.write(reinterpret_cast<char*>(&(entries->at(i).debts)), sizeof(unsigned int));
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
    unsigned int lessons_count = 0, tests_count = 0;
    file.read(reinterpret_cast<char*>(&lessons_count), sizeof(unsigned int));
    file.read(reinterpret_cast<char*>(&tests_count), sizeof(unsigned int));

    for (unsigned int i = 0; i < size; i++) {
        student_entry student;

        unsigned int fio_length = 0;
        file.read(reinterpret_cast<char*>(&fio_length), sizeof(unsigned int));
        student.fio.resize(fio_length);
        file.read(&student.fio[0], fio_length);
        file.read(reinterpret_cast<char*>(&student.group), sizeof(unsigned int));

        for (unsigned int e = 0; e < lessons_count; e++) {
            unsigned int grade = 0;
            file.read(reinterpret_cast<char*>(&grade), sizeof(unsigned int));
            student.exam_grades.push_back(grade);
        }

        for (unsigned int e = 0; e < tests_count; e++) {
            bool test_result = 0;
            file.read(reinterpret_cast<char*>(&test_result), sizeof(char));
            student.test_results.push_back(test_result);
        }

        file.read(reinterpret_cast<char*>(&student.debts), sizeof(unsigned int));

        entries->push_back(student);
    }
    file.close();

    currentFile = fileName;
    clearScreen();
    coutWithColor(colors::LIGHT_GREEN, "Успешно загрузил " + to_string(size) + " записей\n");
}

unsigned int findMaxNameLength(vector<student_entry>* entries, unsigned int size) {
    size_t maxLength = 0;
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
    cout << "Номер группы: " << entry->group << endl;

    for (unsigned int i = 0; i < lessons_size; i++) {
        cout << "Оценка по экзамену по " << lessons_map_case[i] << ": " << entry->exam_grades.at(i) << endl;
    }

    for (unsigned int i = 0; i < tests_size; i++) {
        cout << "Результат зачета по " << lessons_map_case[i] << ": " << (entry->test_results.at(i) ? "зачет" : "незачет") << endl;
    }

    cout << "Количество задолженностей: " << entry->debts << endl;
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
    cout << "Студент" << addSpaces("", maxNameLength - 7) << "|Номер группы" << "|Задолженности" << endl;
    for (unsigned int i = 0; i < size; i++) {
        cout << addSpaces(entries->at(i).fio, maxNameLength) << "|" << addSpaces(to_string(entries->at(i).group), 12) << "|";
        cout << addSpaces(to_string(entries->at(i).debts), 12) << endl;
    }
    cout << endl;
}

void printGroupSummary(vector<student_entry>* entries) {
    unsigned int size = entries->size();
    vector<unsigned int> groups;
    vector<unsigned int> groups_students;
    unsigned int unique_groups = 0;
    for (unsigned int i = 0; i < size; i++) {

        unsigned int groups_size = groups.size();
        bool contains = false;
        int g_index = 0;
        for (unsigned int g = 0; g < groups_size; g++) {
            contains = groups.at(g) == entries->at(i).group;
            if (contains) {
                g_index = g;
                break;
            }
        }
        if (!contains) {
            groups.push_back(entries->at(i).group);
            groups_students.push_back(1);
            unique_groups++;
        }
        else {
            groups_students.at(g_index)++;
        }
    }

    string* groups_selection = new string[unique_groups];
    for (unsigned int i = 0; i < unique_groups; i++) {
        groups_selection[i] = to_string(groups.at(i));
    }

    coutWithColor(colors::LIGHT_YELLOW, "Выберите группу\n");
    int selection = displaySelection(groups_selection, unique_groups) - 1;
    
    unsigned int students = groups_students.at(selection);
    coutWithColor(colors::CYAN, "\nИнформация по группе " + groups_selection[selection]);
    coutWithColor(colors::LIGHTER_BLUE, "\nКоличество студентов: " + to_string(students) + "\n");

    double globalAverage = 0;

    unsigned int maxNameLength = max(findMaxNameLength(entries, students), (unsigned int)7);
    cout << "Студент" << addSpaces("", maxNameLength - 7) << "|Номер группы" << "|Задолженности" << "|Средний балл" << endl;
    for (unsigned int i = 0; i < students; i++) {
        cout << addSpaces(entries->at(i).fio, maxNameLength) << "|" << addSpaces(to_string(entries->at(i).group), 12) << "|";

        double average = 0;
        for (int k = 0; k < lessons_size; k++) {
            average += entries->at(i).exam_grades.at(k);
            globalAverage += entries->at(i).exam_grades.at(k);
        }
        average /= lessons_size;

        cout << addSpaces(to_string(entries->at(i).debts), 13) << "|" << addSpaces(to_string(average), 12) << endl;
    }
    cout << "\nСредний балл по группе: " << globalAverage / (students * lessons_size) << endl;

    coutWithColor(colors::LIGHT_YELLOW, "\nНажмите любую клавишу, чтобы продолжить\n");
    _getch();

    clearScreen();
}

bool fio_compare(student_entry entry1, student_entry entry2) {
    return entry1.fio > entry2.fio;
}

bool debt_compare(student_entry entry1, student_entry entry2) {
    return entry1.debts < entry2.debts;
}

bool group_compare(student_entry entry1, student_entry entry2) {
    return entry1.group < entry2.group;
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

bool fio_match(student_entry entry, string search) {
    return entry.fio.find(search) != string::npos;
}

bool debt_match(student_entry entry, string search) {
    return to_string(entry.debts).find(search) != string::npos;
}

bool group_match(student_entry entry, string search) {
    return to_string(entry.group).find(search) != string::npos;
}

void entrySearch(vector<student_entry>* entries, SearchFunction sortFunction, string search) {
    unsigned int size = entries->size();
    for (unsigned int i = 0; i < size; i++) {
        entries->at(i).valid = sortFunction(entries->at(i), search);
    }
    entries->erase(remove_if(entries->begin(), entries->end(), isInvalid), entries->end());
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
                string input = displayWarningWithInput(colors::YELLOW, "Файл существует и в нем есть данные, перезаписать?\n");
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
        string input = displayWarningWithInput(colors::YELLOW, "Вы уверены, что хотите удалить?\n");
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
    coutWithColor(colors::CYAN, "\nСписок файлов:\n");
    size_t maxLen = 0;
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

bool isInvalidUser(user_entry entry) {
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

    string input = displayWarningWithInput(colors::YELLOW, "Вы уверены, что хотите удалить?\n");
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

string get_new_userName(vector<user_entry>* users, string previousName) {
    string login;
    unsigned int size = users->size();
    bool user_exists;
    while (true) {
        login = inputData("Введите имя пользователя: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64, previousName);
        user_exists = false;
        for (unsigned int i = 0; i < size; i++) {
            user_exists = users->at(i).login == login;
            if (user_exists) {
                break;
            }
        }
        if (!user_exists) {
            break;
        }
        else {
            coutWithColor(colors::LIGHT_RED, "Пользователь с таким именем уже существует, используйте другое имя\n");
        }
    }
    return login;
}

void add_user(vector<user_entry>* users) {
    user_entry new_user;
    unsigned int size = users->size();
    new_user.login = get_new_userName(users, "");
    new_user.password = hash<string>{}(inputPassword());
    coutWithColor(colors::LIGHT_YELLOW, "\nВыберите роль пользователя:\n");
    new_user.access_level = displaySelection(new string[6]{ "1.Пользователь", "2.Администратор" }, 2) - 1;
    users->push_back(new_user);
    clearScreen();
    coutWithColor(colors::LIGHT_GREEN, "Добавил нового " + string(((new_user.access_level == 0) ? "пользователя: " : "администратора: ")) + new_user.login + "\n");
    save_users(users);
}

void deleteUsers(vector<user_entry>* users) {
    unsigned int size = users->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = users->at(i).login;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите пользователей (backspace - выбрать/отменить), (enter - подтвердить)\n");
    bool* selected = displayMultiSelection(selection, size);

    for (unsigned int i = 0; i < size; i++) {
        if (selected[i] && users->at(i).login == currentUser.login) {
            coutWithColor(colors::YELLOW, "Вы выбрали текущую учетную запись, она не будет удалена\n");
            break;
        }
    }

    string input = displayWarningWithInput(colors::YELLOW, "Вы уверены, что хотите удалить?\n");
    int deleted_users = 0;
    if (input == "yes" || input == "y" || input == "1") {
        for (unsigned int i = 0; i < size; i++) {
            users->at(i).valid = !(selected[i] && users->at(i).login != currentUser.login);
            deleted_users += !users->at(i).valid;
        }
        users->erase(remove_if(users->begin(), users->end(), isInvalidUser), users->end());
    }

    delete[] selected;
    clearScreen();
    coutWithColor(colors::LIGHT_GREEN, "Удалил " + to_string(deleted_users) + " пользователя(ей)\n");
    save_users(users);
}

void listUsers(vector<user_entry>* users) {
    unsigned int size = users->size();
    coutWithColor(colors::CYAN, "\nСписок пользователей: " + to_string(size) + '\n');
    for (unsigned int i = 0; i < size; i++) {
        cout << users->at(i).login << ((users->at(i).access_level == 0) ? " (пользователь)\n" : " (администратор)\n");
    }
}

void editUsers(vector<user_entry>* users) {

    unsigned int size = users->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = users->at(i).login;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите пользователя\n");
    user_entry* user = &users->at(displaySelection(selection, size) - 1);

    coutWithColor(colors::LIGHT_GREEN, "Редактирую пользователя: " + user->login + "\n");
    user->login = get_new_userName(users, user->login);
    user->password = hash<string>{}(inputPassword());
    if (user->login != currentUser.login) {
        coutWithColor(colors::LIGHT_YELLOW, "\nВыберите новую роль пользователя:\n");
        user->access_level = displaySelection(new string[6]{ "1.Пользователь", "2.Администратор" }, 2) - 1;
    }
    clearScreen();
    save_users(users);
}

void save_users(vector<user_entry>* users) {
    ofstream file(userInfo_fileName, ios::out | ios::binary);
    unsigned int size = users->size();
    file.write(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        file.write(reinterpret_cast<char*>(&users->at(i).access_level), sizeof(char));
        file.write(reinterpret_cast<char*>(&users->at(i).password), sizeof(size_t));

        // determine the size of the string
        unsigned int login_length = users->at(i).login.length();
        // write string size
        file.write(reinterpret_cast<char*>(&login_length), sizeof(unsigned int));
        // and actual string
        file.write(users->at(i).login.data(), login_length);
    }
    file.flush();
    file.close();
    coutWithColor(colors::LIGHT_GREEN, "Сохранил файл с пользователями\n");
}

void read_users(vector<user_entry>* users) {
    unsigned int size = 0;
    users->clear();
    ifstream file(userInfo_fileName, ios::in | ios::binary);
    file.read(reinterpret_cast<char*>(&size), sizeof(unsigned int));
    for (unsigned int i = 0; i < size; i++) {
        user_entry user;
        file.read(reinterpret_cast<char*>(&user.access_level), sizeof(char));
        file.read(reinterpret_cast<char*>(&user.password), sizeof(size_t));
        unsigned int login_length = 0;
        file.read(reinterpret_cast<char*>(&login_length), sizeof(unsigned int));
        user.login.resize(login_length);
        file.read(&user.login[0], login_length);
        users->push_back(user);
    }
    file.close();
}

void login(vector<user_entry>* users) {
    
    const fs::path login_info{userInfo_fileName};
    if (!fs::exists(login_info)) {
        user_entry admin;
        admin.access_level = 1;
        admin.login = string("admin");
        admin.password = hash<string>{}("admin");
        users->push_back(admin);
        coutWithColor(colors::LIGHT_GREEN, "Первый запуска, создал администратора (пароль: admin)\n");
        save_users(users);
    }

    read_users(users);

    unsigned int size = users->size();
    string* selection = new string[size];
    for (unsigned int i = 0; i < size; i++) {
        selection[i] = users->at(i).login;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Выберите пользователя\n");
    int selected = displaySelection(selection, size);

    size_t pass_hash = 0;
    int attempts = 0;
    while (true) {
        if ((pass_hash = hash<string>{}(inputPassword())) != users->at(selected - 1).password) {
            coutWithColor(colors::LIGHT_RED, "Неправильный пароль\n");
            attempts++;
            if (attempts > 3) {
                coutWithColor(colors::RED, "Больше 3х неудачных попыток ввода\n");
                Sleep(1000);
                exit(-100);
            }
        }
        else {
            clearScreen();
            coutWithColor(colors::LIGHT_GREEN, "Успешная авторизация, добро пожаловать, " + users->at(selected - 1).login + "\n");
            break;
        }
    }
    currentUser = users->at(selected - 1);
}