#include<iostream>
#include "Lab6_task10.h"
using namespace std;
int main()
{
    double** matrix;
    int size;
    cout << "Enter matrix size: ";
    cin >> size;
    matrix = new double* [size];
    for (int i = 0; i < size; i++) { 
        matrix[i] = new double[size];
    };
    
    cout << "Enter matrix elements:\n";
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            cin >> matrix[i][j];

    printMatrix(size, matrix);
    return 0;
}

bool isSkewSymmetric(int size,  double** matrix)
{
    bool notSymmetric = 0;
    bool skewed = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (matrix[i][j] != matrix[j][i]) {
                notSymmetric = 1;
                break;
            } else if (matrix[i][j] == -matrix[j][i])
                skewed = 1;
        }
    }
    return !notSymmetric && skewed;
}

void printMatrix(int size, double** matrix)
{
    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}
