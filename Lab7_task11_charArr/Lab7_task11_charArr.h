#include <string>
#include <cstring>

char* inputData_char(const char* message, char* allowedChars, int allowedChars_size, const int maxStringSize);
void alphabeticSort_char(char** words, unsigned int numberOfWords);
char* substr(char* str, unsigned int start, unsigned int end);
int myStrcmp(char* s1, char* s2);
bool compare2Words(char* word1, char* word2);