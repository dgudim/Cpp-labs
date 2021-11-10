#include "../genericFunctions.h"
using namespace std;

int main()
{
    double a;
    double z;
    double x;
    double func_tempVar;

    while (true) {

        a = inputData("Please, input a = ");
        z = inputData("Please, input z = ");

        if (z < 1) {
            x = z * z;
        }
        else {
            x = z + 1;
        }

        cout << "Please select function" << endl;
        cout << "1 - 2x (default)" << endl;
        cout << "2 - x^2" << endl;
        cout << "3 - x/3" << endl;
        int func = (int)floor(inputData("Function: "));

        string selectedFunc = "";

        switch (func) {
        case 1:
            selectedFunc = "2x";
            func_tempVar = 2 * x;
            break;
        case 2:
            selectedFunc = "x^2";
            func_tempVar = x * x;
            break;
        case 3:
            selectedFunc = "x/3";
            func_tempVar = x / 3.0;
            break;
        default:
            cout << "There is no such function, using default" << endl;
            selectedFunc = "2x";
            func_tempVar = 2 * x;
            break;
        }

        cout << "You chose " << selectedFunc << endl;

        cout << "Result: " << a * log(1 + pow(x, 1.0 / 5.0)) + pow(cos(func_tempVar + 1), 2) << ", have a nice day" << endl;
        continueOrExit();
    }
}
