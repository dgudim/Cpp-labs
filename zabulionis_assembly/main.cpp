#include "../genericFunctions.h"

extern "C" int f1_asm(int x);
asm (R"(
    .global f1_asm

x_less_than_2:
    movl %edi, %eax   #copy edi - x, to eax
    imul $7,   %eax   #multiply by 7
    subl $44,  %eax   #eax - 44
    neg        %eax   #invert 
    ret

x_equals_2:
    movl %edi, %eax   #copy edi - x, to eax
    subl $5,   %eax   #eax - 5
    imul %eax, %eax   #second power
    imul %eax, %eax   #forth power

    movl $9,   %ecx   #divisor
    cdq               #Convert Double to Quad, needed for division
    idivl %ecx        #divide by ecx

    ret

x_more_than_2:
    movl %edi, %eax   #copy edi - x, to eax
    imul %eax, %edi   #compute square, it's now stored in edi
    imul $5,   %eax   #multiply eax by 5
    add  $12,  %eax   #add 12
    subl %edi, %eax   #substract square stored in edi
    ret

f1_asm:
    cmp $2, %edi       #compare edi with 2
    jg  x_more_than_2  #jump when greater than
    jl  x_less_than_2  #jump when less than
    jmp x_equals_2     #else x = 2

)");
extern "C" int f2_asm(int from, int to);
asm (R"(
    .global f2_asm

loop:
    movl %eax, %ecx   #copy eax - i to ecx
    subl $5,   %ecx   #substract 5
    imul %ecx, %ecx   #square
    subl %eax, %ecx   #substract current iteration
    subl $1,   %ecx   #substract 1
    add  %ecx, %esi   #accumulate to esi
    cmp  %eax, %ebx   #compare our current iteration with end
    je   end          #our iteration is equal to end value, exit function
    inc  %eax         #increment eax
    jmp loop

f2_asm:
    movl %edi, %eax   #copy edi - 'from', to eax, this will be our current i value
    movl %esi, %ebx   #copy esi - 'to',   to ebx
    movl $0,   %esi   #reset accumulator
    jmp loop

end:
    movl %esi, %eax   #copy our accumulator to eax for returning
    ret

)");

using namespace std;

void printResult(std::string functionName, int x, int value) {
    coutWithColor(colors::LIGHT_GREEN, "The value of the function");
    coutWithColor(colors::BLUE, " " + functionName + " ");
    coutWithColor(colors::LIGHT_GREEN, "at");
    coutWithColor(colors::LIGHT_YELLOW, " x");
    coutWithColor(colors::LIGHT_GREEN, " = ");
    coutWithColor(colors::LIGHT_PURPLE, std::to_string(x));
    coutWithColor(colors::LIGHT_GREEN, " is:");
    coutWithColor(colors::BLUE, " " + functionName + "(");
    coutWithColor(colors::LIGHT_PURPLE, std::to_string(x));
    coutWithColor(colors::BLUE, ") ");
    coutWithColor(colors::LIGHT_GREEN, "= ");
    coutWithColor(colors::LIGHTER_BLUE, std::to_string(value));
    waitForButtonInput("\n\nPress any key to return\n");
}

void f1() {
    int x = inputDouble("Input x: ", false);
    int value = f1_asm(x);
    printResult("F1", x, value);
}

void f2() { 
    int from = 1;
    int to = std::clamp((int)inputDouble("Input x(to): ", false), -5, 10); 
    if (to < from) {
        std::swap(from, to);
    }
    int value = f2_asm(from, to);
    printResult("F2", to, value);
}

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
