#include "Lab6_task10.h"
#include "../genericFunctions.h"
using namespace std;

int main() {
    double** matrix;
    int size;
    while (true) {
        size = (int)inputDouble("Введите размер квадратной матрицы: ", false);
        if (size <= 0) {
            coutWithColor(colors::RED, "Размер не может быть меньше иле или равен 0, устанавливаю размер 3\n");
            size = 3;
        }
        matrix = inputMatrix(size);
        printMatrix(size, matrix);
        cout << "Матрица " << (isSymmetric(size, matrix) ? "":"не ") << "симметрична относительно побочной диагонали" << endl;
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
    }

    cout << "Введите элементы матрицы:\n";
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = inputDouble("");
    return matrix;
}

void printMatrix(int size, double** matrix)
{
    int maxLen = 0;
    int maxElem = 0;
    int minElem = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++) {
            maxLen = max((int)doubleToString(matrix[i][j]).length(), maxLen);
            maxElem = max((int)matrix[i][j], maxElem);
            minElem = min((int)matrix[i][j], minElem);
        }
    }

    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++) {
            coutWithColor(mapToColor(matrix[i][j], minElem, maxElem), addSpaces(doubleToString(matrix[i][j]), maxLen) + " ");
        }
        cout << "\n";
    }
}

bool isSymmetric(int size, double** matrix)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (matrix[i][j] != matrix[size - j - 1][size - i - 1]) {
                return false;
            }
        }      
    }
    return true;
}
