#include "Lab6_task10.h"
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
        while (cin.get() != '\n');
        cout << "Пожалуйста, используйте числа" << endl;
    }
    return toReturn;
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void coutWithColor(int color, string message) {
    setConsoleColor(color);
    cout << message << flush;
    setConsoleColor(7);
}

string doubleToString(double value) {
    ostringstream out;
    out.precision(9);
    out << std::fixed << value;
    string strOut = out.str();
    char currChar = strOut[strOut.length() - 1];
    while ((currChar == '0' || currChar == '.') && strOut.length() > 1) {
        strOut.erase(strOut.length() - 1, 1);
        if (currChar == '.') {
            break;
        }
        currChar = strOut[strOut.length() - 1];
    }
    return strOut;
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

bool continueOrExit()
{
    cout << "Продолжить?" << endl;
    string input;
    cin >> input;
    return input == "yes" || input == "y" || input == "1";
}