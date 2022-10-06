using namespace std;
#include "../genericFunctions.h"
#include <math.h>

double calculate(double z, double y, double x) {
    double zSquared = z * z;
    return pow(abs(cos(x) - cos(y)), (1 + 2 * pow(sin(y), 2))) * (1 + z + zSquared / 2.0 + zSquared * z / 3.0 + pow(zSquared, 2) / 4.0);
}

int main()
{
    while (true) {
        cout << "\n\tОтвет: " << calculate(
            inputDouble("Пожалуйста, введите z = "), 
            inputDouble("Пожалуйста, введите y = "), 
            inputDouble("Пожалуйста, введите x = ")) << ", хорошего дня\n" << endl;
        continueOrExit();
    }
    return 0;
}
