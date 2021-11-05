#include<iostream>
#include<string>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set( "utf-8" )

bool isSymmetric(int size, double** matrix);
double inputData(std::string message);
double** inputMatrix(int size);
void printMatrix(int size, double** matrix);
bool continueOrExit();