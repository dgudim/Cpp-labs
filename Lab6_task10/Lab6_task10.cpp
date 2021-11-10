#include "Lab6_task10.h"
#include "../genericFunctions.h"
using namespace std;

int main()
{
    SetConsoleOutputCP(65001);
    double** matrix;
    int size;
    while (true) {
        size = (int)inputData("Введите размер матрицы: ");
        if (size <= 0) {
            coutWithColor(4, "Размер не может быть меньше иле или равен 0, устанавливаю размер 2\n");
            size = 2;
        }
        matrix = inputMatrix(size);
        printMatrix(size, matrix);
        cout << (isSymmetric(size, matrix) ? string("Матрица симметрична относительно побочной диагонали") : string("Матрица не симметрична относительно побочной диагонали")) << endl;
        for (int i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        continueOrExit();
    }
    return 0;
}

double** inputMatrix(int size){
    double** matrix;
    matrix = new double* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
    };

    cout << "Введите элементы матрицы:\n";
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = inputData("");
    return matrix;
}

void printMatrix(int size, double** matrix)
{
    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++) {
            coutWithColor((int)(matrix[i][j]) % 11 + 1, doubleToString(matrix[i][j]) + " ");
        }
        cout << "\n";
    }
}

bool isSymmetric(int size, double** matrix)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] != matrix[size - j - 1][size - i - 1]) {
                return false;
            }
        }      
    }
    return true;
}