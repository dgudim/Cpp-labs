#include <iostream>
#include <string>
#include <sstream>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set( "utf-8" )

bool isSymmetric(int size, double** matrix);
double** inputMatrix(int size);
void printMatrix(int size, double** matrix);
