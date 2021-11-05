#include<iostream>
#include<string>
#include<sstream>
#define NOMINMAX
#include <windows.h>

#pragma execution_character_set( "utf-8" )

bool isSkewSymmetric(int size, double** matrix);
double inputData(std::string message);
void setConsoleColor(int color);
void coutWithColor(int color, std::string message);
std::string doubleToString(double value);
double** inputMatrix(int size);
void printMatrix(int size, double** matrix);
bool continueOrExit();