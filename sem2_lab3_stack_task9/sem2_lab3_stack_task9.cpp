#include <iostream>
#include "../genericFunctions.h"
using namespace std;

struct StackNode {
    int data;
    StackNode* next;
};

StackNode* add(StackNode* root, int data) {
    StackNode* node = new StackNode;
    node->data = data;
    node->next = root;
    return node;
}

void view(StackNode* node) {
    StackNode* curr_node = node;
    while (curr_node) {
        cout << curr_node->data << endl;
        curr_node = curr_node->next;
    }
}

void view_rec(StackNode* node) {
    if (node) {
        cout << node->data << endl;
        view_rec(node->next);
    }
}

void view_rec_reverse(StackNode* node) {
    if (node) {
        view_rec_reverse(node->next);
        cout << node->data << endl;
    }
}

void del(StackNode** p, int n) {
    StackNode* curr_node = *p;
    while (*p && n > 0) {
        curr_node = *p;
        *p = (*p)->next;
        delete curr_node;
        n--;
    }
}

int remove_between_min_max(StackNode* node) {
    StackNode* min = node;
    StackNode* max = node;
    StackNode* curr_node = node;
    bool minFirst = false;
    while (curr_node) {
        if (curr_node-> data < min->data) {
            min = curr_node;
            minFirst = false;
        }else
        if (curr_node->data > max->data) {
            max = curr_node;
            minFirst = true;
        }
        curr_node = curr_node->next;
    }
    if (min == max || min->next == max || max->next == min) {
        coutWithColor(colors::LIGHT_YELLOW, "Максимальный и минимальный стоят рядом, нечего убирать\n");
        return 0;
    }
    if (!minFirst) {
        StackNode* temp = min;
        min = max;
        max = temp;
    }
    int deletedElems = 0;
    StackNode* next_node = min->next;
    min->next = max;
    while (next_node != max) {
        deletedElems++;
        curr_node = next_node;
        next_node = curr_node->next;
        delete curr_node;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Удалил " + to_string(deletedElems) + " элемента(ов) между максимальным("
        + to_string(max->data) + ") и минимальным(" + to_string(min->data) + ") элементами\n");
    return deletedElems;
}

int main()
{
    StackNode* root = nullptr;
    int n, size = 0;
    bool manual;
    while (true) {
        if (!root) {
            coutWithColor(colors::LIGHT_RED, "Стек пустой\n");
        }
        else {
            coutWithColor(colors::LIGHT_BLUE, "--- Стек --- (" + to_string(size) + ")\n");
            coutWithColor(colors::LIGHT_GREEN, "Обычный вывод ---\n");
            view(root);
            coutWithColor(colors::LIGHT_GREEN, "Наоборот рекурсивно ---\n");
            view_rec_reverse(root);
            coutWithColor(colors::LIGHT_GREEN, "Рекурсивно ---\n");
            view_rec(root);
        }
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        switch (displaySelection(new string[5]{ "1.Добавить данные в стек", "2.Удалить n элементов", "3.Удалить элементы между максимальным и минимальным элементами", "4.Выйти"}, 4)) {
        case 1: 
            n = (int)inputData("Сколько элементов добавить? : ", false);
            if (n <= 0) {
                system("CLS");
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество элементов\n");
                break;
            }
            size += n;
            coutWithColor(14, "\nКак бы вы хотели ввести элементы?\n");
            manual = displaySelection(new string[2]{ "1.Случайно", "2.Вручную" }, 2) == 2;
            for (int i = 0; i < n; i++) {
                if (manual) {
                    root = add(root, inputData(""));
                }
                else {
                    root = add(root, rand() % 100 / 2 - 25);
                }
            }
            system("CLS");
            coutWithColor(colors::LIGHTER_BLUE, "Добавил " + to_string(n) + " элементов\n");
            break;
        case 2:
            if (root) {
                n = (int)inputData("Сколько элементов удалить? : ", false);
                system("CLS");
                if (n <= 0) {
                    coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, вы ввели число <= 0\n");
                    break;
                }
                del(&root, n);
                size -= n;
                coutWithColor(colors::LIGHTER_BLUE, "Удалил " + to_string(min(size, n)) + " элементов");
            }
            else {
                system("CLS");
                coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, стек пустой\n");
            }
            break;
        case 3:
            system("CLS");
            if (root) {
                size -= remove_between_min_max(root);
            }
            else {
                coutWithColor(colors::LIGHTER_BLUE, "Нечего удалять, стек пустой\n");
            }    
            break;
        case 4: 
            if (root)
                del(&root, size);
            return 0;
        }
    }
    return 1;
}