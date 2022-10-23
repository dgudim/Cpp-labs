#include "../genericFunctions.h"
#include <iostream>
#include <string>
using namespace std;

// 36
string conversion_arr = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int getNumSignificantDigits(int radix, double eps) {
    return (int)ceil(log(1 / eps) / log(radix));
}

string& convertInt(int num, int radix, string& buf) {
    if (num == 0) {
        return buf;
    }
    int remainder = num % radix;
    buf.insert(0, 1, conversion_arr[remainder]);
    return convertInt(num / radix, radix, buf);
}

string& convertFract(double num, int radix, int num_digits, string& buf) {
    if (num_digits == 0) {
        return buf;
    }
    double mul = num * radix;
    int int_part = floor(mul);
    double frac_part = mul - int_part;

    buf += conversion_arr[int_part];
    return convertFract(frac_part, radix, num_digits - 1, buf);
}

int main() {
    coutWithColor(colors::LIGHT_GREEN, "Input initial data:\n");
    double num = inputDouble("    Initial decimal number: ");
    int radix = (int)inputDouble("    Target radix: ");
    double eps = inputDouble("    Target precision (epsilon_sup): ");

    cout << "\n";

    int int_part = floor(num);
    double frac_part = num - int_part;

    if (radix >= 35 || radix < 2) {
        cout << "Invalid radix, converting to binary\n";
        radix = 2;
    }

    string str;
    string str2;

    int num_digits = getNumSignificantDigits(radix, eps);

    coutWithColor(colors::YELLOW, "Results:\n");
    cout << "    Number of significant digits: ";
    coutWithColor(colors::LIGHT_PURPLE, to_string(num_digits));
    cout << " (corresponds to epsilon_sup: ";
    coutWithColor(colors::LIGHT_BLUE, to_string(eps));
    cout << ")\n";

    cout << "    The result of converting: ";
    coutWithColor(colors::LIGHT_GREEN, to_string(num) + "₁₀");
    cout << " to radix ";
    coutWithColor(colors::PURPLE, to_string(radix));
    cout << ": ";
    coutWithColor(colors::GREEN, convertInt(int_part, radix, str) + "," + convertFract(frac_part, radix, num_digits, str2));
    waitForButtonInput("\n\nPress any button to close the window...");
}