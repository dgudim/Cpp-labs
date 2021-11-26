#include <filesystem>
#include <fstream>
#include <map>

struct student_entry;

void clearScreen();

void inputEntry(student_entry* entry);
void editEntry(student_entry* entry);

void write_entries(std::vector<student_entry>* entries, std::string fileName);
void read_entries(std::vector<student_entry>* entries, std::string fileName);

unsigned int findMaxNameLength(std::vector<student_entry>* entries, unsigned int size);

void printEntry(student_entry* entry);
void printSummary(std::vector<student_entry>* entries);

void alphabeticSort(std::vector<student_entry>* entries);
void gradeSort(std::vector<student_entry>* entries);

void createFile();
void loadFromFile(std::vector<student_entry>* entries);
unsigned int getStats(std::string path);
void deleteFiles();
void listFiles();

bool isInvalid(student_entry entry);

void viewEntries(std::vector<student_entry>* entries);
void deleteEntries(std::vector<student_entry>* entries);
bool addEntries(std::vector<student_entry>* entries);
void editEntries(std::vector<student_entry>* entries);
void edit(std::vector<student_entry>* entries);