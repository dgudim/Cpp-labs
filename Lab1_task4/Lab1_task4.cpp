#include <iostream>
#include <limits>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

int errCount = 0;

double inputData(string varName) {
    cout << "Please input " << varName << ". " << varName << " = ";
    double toReturn;
    while (!(cin >> toReturn)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please use numbers" << endl;
        errCount++;
        if (errCount == 10) {
            cout << "Please stop >_<" << endl;
        }
    }
    return toReturn;
}

double calculate(double z, double y, double x) {
    double firstHalf = abs(cos(x) - cos(y));
    double secondHalf = (1 + 2 * pow(sin(y), 2));
    double zSquared = z * z;
    return pow(firstHalf, secondHalf) * (1 + z + zSquared / 2.0 + zSquared * z / 3.0 + pow(zSquared, 2) / 4.0);
}

int main()
{
    while (true) {
        cout << "\n\tResult: " << calculate(inputData("z"), inputData("y"), inputData("x")) << ", have a nice day\n" << endl;
        system("pause");
    }
    return 0;
}
