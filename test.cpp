#include <iostream>
#include "genericFunctions.h"
using namespace std;

int main()
{
    while(true){
        cout << _getch() << endl;
    }
    
    //cout << _getch() << endl;
    COORD coord = getConsoleCursorPosition();
    setConsoleCursorPosition(coord.X, coord.Y + 4);
    cout << "Testing" << endl;
    return 0;
}