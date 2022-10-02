#include "../genericFunctions.h"
#include "lab_selector.h"
#include <iomanip>

// entrypoint
int main() {
    while (true) {
        clearScreen();
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=MENU=-=-=-=-=-=-=-\n");
        coutWithColor(colors::LIGHT_PURPLE, "Choose lab pack\n");
        switch (displaySelection(new string[4]{
            "1.Labs 1",
            "2.Labs 2",
            "3.Labs 3",
            "4.Exit"
            }, 4)) {
        case 1:
            labs1();
            break;
        case 2:
            labs2();
            break;
        case 3:
            labs3();
            break;
        case 4:
            return 0;
        }
    }
    return 0;
}

// pack 1
void labs1() {
    int i = 0;
    while (true) {
        clearScreen();
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=LABS 1=-=-=-=-=-=-=-\n");
        coutWithColor(colors::LIGHT_PURPLE, "Choose task\n");

        int choice = displaySelection(new string[10]{
            "1.Minutes to hours and minutes",
            "2.Print integers",
            "3.Days to weeks and days",
            "4.Centimeters to feet and inches",
            "5.Addemup",
            "6.Addemup squared",
            "7.Cube the number",
            "8.Modulus",
            "9.Fahrenheit to celcius and kelvin",
            "10.Exit to main menu"
            }, 10);

        switch (choice) {
        case 1:
            M_to_HM();
            break;
        case 2:
            printIntegers();
            break;
        case 3:
            D_to_WD();
            break;
        case 4:
            Cm_to_feet_inches();
            break;
        case 5:
            addemup(false);
            break;
        case 6:
            addemup(true);
            break;
        case 7:
            while (true) {
                double target = inputData("Input an integer: ", false);
                if (target <= 0) {
                    break;
                }
                cout << "The cube is: ";
                coutWithColor(colors::LIGHT_BLUE, std::to_string(cube(target)) + "\n");
            }
        case 8:
            modulus();
            break;
        case 9:
            fahrenheitToCelsiusKelvin();
            break;
        case 10:
            return;
        }
    }
}

#define MINUTES_IN_HOUR 60

void M_to_HM() {
    while (true) {
        int minutes = inputData("Input number of minutes: ", false);
        if (minutes <= 0) {
            return;
        }
        int hours = minutes / MINUTES_IN_HOUR;
        int left_minutes = minutes - hours * MINUTES_IN_HOUR;
        coutWithColor(colors::LIGHT_GREEN, std::to_string(minutes));
        cout << " minutes are ";
        coutWithColor(colors::LIGHT_BLUE, std::to_string(hours));
        cout << " hours and ";
        coutWithColor(colors::LIGHT_PURPLE, std::to_string(left_minutes));
        cout << " minutes\n";
    }
}

void printIntegers() {
    while (true) {
        int start = inputData("Input start number: ", false);
        if (start <= 0) {
            return;
        }
        for (int i = start; i <= start + 10; i++) {
            coutWithColor(mapToColor(i), std::to_string(i) + " ");
        }
        cout << std::endl;
    }
}

#define DAYS_IN_WEEK 7

void D_to_WD() {
    while (true) {
        int days = inputData("Input number of days: ", false);
        if (days <= 0) {
            return;
        }
        int weeks = days / DAYS_IN_WEEK;
        int left_days = days - weeks * DAYS_IN_WEEK;
        coutWithColor(colors::LIGHT_GREEN, std::to_string(days));
        cout << " days are ";
        coutWithColor(colors::LIGHT_BLUE, std::to_string(weeks));
        cout << " weeks and ";
        coutWithColor(colors::LIGHT_PURPLE, std::to_string(left_days));
        cout << " days\n";
    }
}

void Cm_to_feet_inches() {
    while (true) {
        double cm = inputData("Input height in centimeters: ", false);
        if (cm <= 0) {
            return;
        }
        double inches = cm / 2.54;
        int feet = inches / 12;
        inches -= feet * 12;

        coutWithColor(colors::LIGHT_GREEN, std::to_string(cm));
        cout << " cm = ";
        coutWithColor(colors::LIGHT_BLUE, std::to_string(feet));
        cout << " feet ";
        coutWithColor(colors::LIGHT_PURPLE, std::to_string(inches));
        cout << " inches";
        cout << std::endl;
    }
}

void addemup(bool square) {
    while (true) {
        int target = inputData("Input target integer: ", false);
        if (target <= 0) {
            return;
        }
        int sum = 0;
        for (int i = 1; i <= target; i++) {
            sum += square ? i * i : i;
        }

        cout << "The sum is: ";
        coutWithColor(colors::LIGHT_BLUE, std::to_string(sum) + "\n");
    }
}

double cube(double num) {
    return num * num * num;
}

void modulus() {
    int second_operand = inputData("Input an integer to serve as the second operand: ", false);
    while (true) {
        int first_operand = inputData("Input an integer to serve as the first operand: ", false);
        if (first_operand <= 0) {
            return;
        }
        coutWithColor(colors::LIGHT_BLUE, std::to_string(first_operand));
        cout << " % ";
        coutWithColor(colors::LIGHT_GREEN, std::to_string(second_operand));
        cout << " = ";
        coutWithColor(colors::LIGHT_PURPLE, std::to_string(first_operand % second_operand) + "\n");
    }
}

// TODO: exit on letters
void fahrenheitToCelsiusKelvin() {
    while (true) {
        double fahrenheit = inputData("Input temperature in fahrenheit: ", false);
        double celcius = 5.0 / 9.0 * (fahrenheit - 32.0);
        double kelvin = celcius + 273.16;
        cout << "that would be " << std::fixed << std::setprecision(2);
        setConsoleColor(colors::LIGHT_BLUE);
        cout << celcius;
        setConsoleColor(colors::DEFAULT);
        cout << " celcius | ";
        setConsoleColor(colors::LIGHT_GREEN);
        cout << kelvin;
        setConsoleColor(colors::DEFAULT);
        cout << " kelvin\n";

        // restore default precision
        cout << std::cout.precision();
    }
}

// pack 2
void labs2() {
    coutWithColor(colors::RED, "Not implemented\n");
    waitForButtonInput("Press any key to return\n");
    return;
}

// pack 3
void labs3() {
    coutWithColor(colors::RED, "Not implemented\n");
    waitForButtonInput("Press any key to return\n");
    return;
}