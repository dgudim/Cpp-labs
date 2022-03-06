#include <filesystem>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

struct student_entry;
struct user_entry;

typedef bool(*SortFunction)(student_entry, student_entry);
typedef bool(*SearchFunction)(student_entry, std::string);

bool displayUserMenu(std::vector<user_entry>* users, std::vector<student_entry>* entries);
bool displayAdminMenu(std::vector<user_entry>* users, std::vector<student_entry>* entries);

void inputEntry(student_entry* entry);
void editEntry(student_entry* entry);

void write_entries(std::vector<student_entry>* entries, std::string fileName);
void read_entries(std::vector<student_entry>* entries, std::string fileName);

unsigned int findMaxNameLength(std::vector<student_entry>* entries, unsigned int size);

void printEntry(student_entry* entry);
void printSummary(std::vector<student_entry>* entries);
void printGroupSummary(std::vector<student_entry>* entries);


bool group_compare(student_entry entry1, student_entry entry2);
bool fio_compare(student_entry entry1, student_entry entry2);
bool debt_compare(student_entry entry1, student_entry entry2);

void sort(std::vector<student_entry>* entries);
void entrySort(std::vector<student_entry>* entries, SortFunction sortFunction);



bool fio_match(student_entry entry, std::string search);
bool debt_match(student_entry entry, std::string search);
bool group_match(student_entry entry, std::string search);

void search(std::vector<student_entry>* entries);
void entrySearch(std::vector<student_entry>* entries, SearchFunction sortFunction, std::string search);



std::string createFile();
void loadFromFile(std::vector<student_entry>* entries);
unsigned int getStats(std::string path);
void deleteFiles();
void listFiles();

bool isInvalid(student_entry entry);
bool isInvalidUser(user_entry entry);

std::string get_new_userName(std::vector<user_entry>* users, std::string previousName);
void save_users(std::vector<user_entry>* users);
void read_users(std::vector<user_entry>* users);
void add_user(std::vector<user_entry>* users);
void deleteUsers(std::vector<user_entry>* users);
void editUsers(std::vector<user_entry>* users);

void listUsers(std::vector<user_entry>* users);

void viewEntries(std::vector<student_entry>* entries);
void deleteEntries(std::vector<student_entry>* entries);
bool addEntries(std::vector<student_entry>* entries);
void editEntries(std::vector<student_entry>* entries);
void edit(std::vector<student_entry>* entries);
void edit_minimal(std::vector<student_entry>* entries);

void login(std::vector<user_entry>* users);