#include "../genericFunctions.h"
#include <iostream>
#include <string>
using namespace std;

char conversion_arr[36]{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

string& convert (int num, int scale_of_notation, string& buf) {
    if (num == 0) {
        return buf;
    }
    int remainder = num % scale_of_notation;
    buf.insert(0, 1, conversion_arr[remainder]);
    return convert(num / scale_of_notation, scale_of_notation, buf);
}

int main()
{
    int num = (int)inputDouble("Введите число: ");
    int scale_of_notation = (int)inputDouble("Введите систему систему счисления: ");

    if (scale_of_notation >= 35 || scale_of_notation < 2) {
        cout << "Прикалываетесь?, перевожу в двоичную\n";
        scale_of_notation = 2;
    }
    
    string str("");
    cout << "Ваше число в " << scale_of_notation << "-ичной системе счисления: " << convert(num, scale_of_notation, str);
}