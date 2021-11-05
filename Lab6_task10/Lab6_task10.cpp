#include "Lab6_task10.h"
using namespace std;
int main()
{
    SetConsoleOutputCP(65001);
    double** matrix;
    int size;
    while (true) {
        size = (int)inputData("Введите размер матрицы: ");
        matrix = inputMatrix(size);
        printMatrix(size, matrix);
        cout << (isSkewSymmetric(size, matrix) ? string("Матрица симметрична относительно побочной диагонали") : string("Матрица не симметрична относительно побочной диагонали")) << endl;
        for (int i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        if (!continueOrExit()) {
            break;
        }
    }
    return 0;
}

double inputData(string message) {
    cout << message << flush;
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
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
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

bool isSkewSymmetric(int size, double** matrix)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (matrix[i][j] != matrix[j][i]) 
                return false;

    return true;
}

bool continueOrExit()
{
    cout << "Продолжить?" << endl;
    string input;
    cin >> input;
    return input == "yes" || input == "y" || input == "1";
}