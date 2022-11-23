#include "../genericFunctions.h"

extern "C" int f1_asm(int x);
asm (R"(
    .global f1_asm
f1_asm:
    movl %edi, %eax #move from edi - x, to eax - return register
    mull %eax, $7
    ret
)");
// extern "C" int f2_asm(int x);

void f1() {
    int x = inputDouble("Input x: ", false);
    int value = f1_asm(x);
    coutWithColor(colors::LIGHT_GREEN, "The value of the function");
    coutWithColor(colors::BLUE, " F1 ");
    coutWithColor(colors::LIGHT_GREEN, "at");
    coutWithColor(colors::LIGHT_YELLOW, " x");
    coutWithColor(colors::LIGHT_GREEN, " = ");
    coutWithColor(colors::LIGHT_PURPLE, std::to_string(x));
    coutWithColor(colors::LIGHT_GREEN, " is:");
    coutWithColor(colors::BLUE, " F1(");
    coutWithColor(colors::LIGHT_PURPLE, std::to_string(x));
    coutWithColor(colors::LIGHT_GREEN, ") = ");
    coutWithColor(colors::LIGHTER_BLUE, std::to_string(value));
    waitForButtonInput("\n\nPress any key to return\n");
}

void f2() { int x = inputDouble("Input x: ", false); }

// entrypoint
int main() {
    while (true) {
        clearScreen();
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=MENU=-=-=-=-=-=-=-\n");
        coutWithColor(colors::LIGHT_PURPLE, "Choose function\n");
        switch (displaySelection({"1.F1", "2.F2", "3.Exit"})) {
        case 1:
            f1();
            break;
        case 2:
            f2();
            break;
        case 3:
            return 0;
        }
    }
    return 0;
}
