#include <filesystem>
#include <fstream>
#include <map>

struct student_entry;

typedef bool(*SortFunction)(student_entry, student_entry);

void clearScreen();

void inputEntry(student_entry* entry);
void editEntry(student_entry* entry);

void write_entries(std::vector<student_entry>* entries, std::string fileName);
void read_entries(std::vector<student_entry>* entries, std::string fileName);

unsigned int findMaxNameLength(std::vector<student_entry>* entries, unsigned int size);

void printEntry(student_entry* entry);
void printSummary(std::vector<student_entry>* entries);

bool avg_grade_compare(student_entry entry1, student_entry entry2);
bool group_compare(student_entry entry1, student_entry entry2);
bool fio_compare(student_entry entry1, student_entry entry2);
bool year_compare(student_entry entry1, student_entry entry2);

void sort(std::vector<student_entry>* entries);
void entrySort(std::vector<student_entry>* entries, SortFunction sortFunction);

std::string createFile();
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