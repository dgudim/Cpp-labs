#include <iostream>
#include "../genericFunctions.h"
using namespace std;

struct QueueNode {
    int data;
    QueueNode* next;
    QueueNode* prev;
};

void add(QueueNode*& root, QueueNode*& tail, int data, bool back) {
    QueueNode* node = new QueueNode;
    node->data = data;
    if (back) {
        node->next = nullptr;
        node->prev = tail;
        if (tail) {
            tail->next = node;
        }
    } else {
        node->next = root;
        node->prev = nullptr;
        if (root) {
            root->prev = node;
        }
    }
    if (!root && !tail) {
        root = node;
        tail = node;
        return;
    }
    if (back) {
        tail = node;
    } else {
        root = node;
    }
}

void view(QueueNode* end_node) {
    QueueNode* curr_node = end_node;
    bool from_back = !curr_node->next;
    while (curr_node) {          
        cout << curr_node->data << " ";
        curr_node = from_back ? curr_node->prev : curr_node->next;
    }
    cout << endl;
}

void del(QueueNode*& root, QueueNode*& tail, int n, bool from_back) {
    QueueNode* p = from_back ? tail : root;
    QueueNode* curr_node = p;
    while (p && n > 0) {
        curr_node = p;
        p = from_back ? p->prev : p->next;
        delete curr_node;
        n--;
    }
    if (p) {
        if (from_back) {
            tail = p;
            tail->next = nullptr;
        }
        else {
            root = p;
            root->prev = nullptr;
        }
    }
    else {
        tail = nullptr;
        root = nullptr;
    }
}

void printQueue(QueueNode*& root, QueueNode*& tail, int size){
    if (!root) {
        coutWithColor(colors::LIGHT_BLUE, "--- Очередь --- (" + to_string(size) + ")\n");
        coutWithColor(colors::LIGHT_RED, "Очередь пустая\n");
    }
    else {
        coutWithColor(colors::LIGHT_BLUE, "--- Очередь --- (" + to_string(size) + ")\n");
        coutWithColor(colors::LIGHT_GREEN, "Обычный вывод ---\n");
        view(root);
        coutWithColor(colors::LIGHT_GREEN, "Наоборот ---\n");
        view(tail);
    }
}

int remove_between_min_max(QueueNode*& root, QueueNode*& tail, int size) {
    QueueNode* min = root;
    QueueNode* max = root;
    QueueNode* curr_node = root;
    bool minFirst = false;
    while (curr_node) {
        if (curr_node->data < min->data) {
            min = curr_node;
            minFirst = false;
        } else if (curr_node->data > max->data) {
            max = curr_node;
            minFirst = true;
        }
        curr_node = curr_node->next;
    }
    
    if (min == max || min->next == max || max->next == min) {
        coutWithColor(colors::LIGHT_YELLOW, "Максимальный и минимальный стоят рядом, нечего убирать\n");
        return 0;
    }

    coutWithColor(colors::BLUE, "Старая очередь\n");
    printQueue(root, tail, size);
    cout << "\n";

    if (!minFirst) {
        QueueNode* temp = min;
        min = max;
        max = temp;
    }
    int deletedElems = 0;
    QueueNode* next_node = min->next;
    min->next = max;
    max->prev = min;
    while (next_node != max) {
        deletedElems++;
        curr_node = next_node;
        next_node = curr_node->next;
        delete curr_node;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Удалил " + to_string(deletedElems) + " элемента(ов) между максимальным("
        + to_string(minFirst ? max->data : min->data) 
        + ") и минимальным(" + to_string(minFirst ? min->data : max->data) + ") элементами\n");
    return deletedElems;
}


int deleteAllNegative(QueueNode*& root, QueueNode*& tail){
     int deletedElems = 0;
     QueueNode* curr_node = root;
     bool delete_ = false;
     while (curr_node) {
         if (curr_node->data < 0){
             if(curr_node->next){
                curr_node->next->prev = curr_node->prev;
             }
             if(curr_node->prev){
                 curr_node->prev->next = curr_node->next;
             }
             delete_ = true;
             deletedElems ++;
         }
         if (delete_){
             QueueNode* temp_node = curr_node;
             if (curr_node == root && root){
                 root = root->next;
             }
             if(curr_node == tail && tail){
                 tail = tail->prev;
             }
             curr_node = curr_node->next;
             delete temp_node;
             delete_ = false;
         } else {
            curr_node = curr_node->next;
         }
     }
     coutWithColor(colors::LIGHT_YELLOW, "Удалил " + to_string(deletedElems) + " элемента(ов)\n");
     return deletedElems;
}

int main()
{
    QueueNode* root = nullptr;
    QueueNode* tail = nullptr;
    int n, size = 0, subs = 0;
    bool manual;
    while (true) {
        printQueue(root, tail, size);
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[7]{
            "1.Добавить данные в очередь с начала",
            "2.Добавить данные в очередь с конца",
            "3.Удалить n элементов с начала",
            "4.Удалить n элементов с конца",
            "5.Удалить элементы между максимальным и минимальным элементами",
            "6.Удалить все отрицательные элементы",
            "7.Выйти" }, 7);
        switch (choise) {
        case 1:
        case 2:
            n = (int)inputData("Сколько элементов добавить? : ", false);
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество элементов\n");
                break;
            }
            size += n;
            coutWithColor(14, "\nКак бы вы хотели ввести элементы?\n");
            manual = displaySelection(new string[2]{ "1.Случайно", "2.Вручную" }, 2) == 2;

            clearScreen();
            coutWithColor(colors::BLUE, "Старая очередь\n");
            printQueue(root, tail, size);
            cout << "\n";

            if (manual){
                 coutWithColor(colors::BLUE, "Введите элементы(" + to_string(n) + "): ");
            }

            for (int i = 0; i < n; i++) {
                if (manual) {
                    add(root, tail, (int)inputData(""), choise == 2);
                }
                else {
                    add(root, tail, rand() % 100 / 2 - 25, choise == 2);
                }
            }

            coutWithColor(colors::LIGHTER_BLUE, "Добавил " + to_string(n) + " элементов с " 
            + (choise == 2 ? "конца\n" : "начала\n"));
            break;
        case 3:
        case 4:
            if (root) {
                n = (int)inputData("Сколько элементов удалить? : ", false);
                clearScreen();
                if (n <= 0) {
                    coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, вы ввели число <= 0\n");
                    break;
                }
                coutWithColor(colors::BLUE, "Старая очередь\n");
                printQueue(root, tail, size);
                cout << "\n";
                del(root, tail, n, choise == 4);
                n = min(n, size);
                size -= n;
                coutWithColor(colors::LIGHTER_BLUE, "Удалил " + to_string(n) + " элементов c "
                + (choise == 4 ? "конца\n" : "начала\n"));
            }
            else {
                clearScreen();
                coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, очередь пустая\n");
            }
            break;
        case 5:
            clearScreen();
            if (root) {
                coutWithColor(colors::BLUE, "Старая очередь\n");
                printQueue(root, tail, size);
                cout << "\n";
                size -= remove_between_min_max(root, tail, size);
            }
            else {
                coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, очередь пустая\n");
            }
            break;
        case 6:
            clearScreen();
            if (root) {
                coutWithColor(colors::BLUE, "Старая очередь\n");
                printQueue(root, tail, size);
                cout << "\n";
                size -= deleteAllNegative(root, tail);
            }
            else {
                coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, очередь пустая\n");
            }
            break;
        case 7:
            if (root)
                del(root, tail, size, false);
            return 0;
        }
    }
    return 1;
}
