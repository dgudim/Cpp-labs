#include "../genericFunctions.h"
#include <algorithm>
#include <iostream>
using namespace std;
using std::function;

struct StackNode {
    int data;
    StackNode *next;
};

StackNode *add(StackNode *root, int data) {
    StackNode *node = new StackNode;
    node->data = data;
    node->next = root;
    return node;
}

void view(StackNode *node) {
    StackNode *curr_node = node;
    while (curr_node) {
        cout << curr_node->data << endl;
        curr_node = curr_node->next;
    }
}

void view_rec(StackNode *node) {
    if (node) {
        cout << node->data << endl;
        view_rec(node->next);
    }
}

void view_rec_reverse(StackNode *node) {
    if (node) {
        view_rec_reverse(node->next);
        cout << node->data << endl;
    }
}

void del(StackNode *&p, int n) {
    StackNode *curr_node = p;
    while (p && n > 0) {
        curr_node = p;
        p = p->next;
        delete curr_node;
        n--;
    }
}

int find_min_max(StackNode *node, StackNode *&min, StackNode *&max) {
    min = node;
    max = node;
    StackNode *curr_node = node;
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
        coutWithColor(colors::LIGHT_YELLOW, "Max and min elements are adjacent\n");
        return 0;
    }
    if (!minFirst) {
        StackNode *temp = min;
        min = max;
        max = temp;
    }
    return 1;
}

int product_between_min_max(StackNode *node) {
    StackNode *min = nullptr;
    StackNode *max = nullptr;
    int res = find_min_max(node, min, max);
    if (res == 0) {
        return 0;
    }
    int product = 1;
    StackNode *next_node = min->next;
    while (next_node != max) {
        product *= next_node->data;
        next_node = next_node->next;
    }
    cout << "The product of the elements between the largest and the smallest element is "
         << product << endl;
    return 0;
}

int remove_between_min_max(StackNode *node) {
    StackNode *min = nullptr;
    StackNode *max = nullptr;
    int res = find_min_max(node, min, max);
    if (res == 0) {
        return 0;
    }

    StackNode *curr_node = node;
    int deletedElems = 0;
    StackNode *next_node = min->next;
    min->next = max;
    while (next_node != max) {
        deletedElems++;
        curr_node = next_node;
        next_node = curr_node->next;
        delete curr_node;
    }
    coutWithColor(colors::LIGHT_YELLOW, "Deleted " + to_string(deletedElems) +
                                            " element(s) between max(" + to_string(max->data) +
                                            ") and min(" + to_string(min->data) +
                                            ") elements\n");
    return deletedElems;
}

void sumAllNegative(StackNode *node) {
    int sum = 0;
    StackNode *curr_node = node;
    while (curr_node) {
        if (curr_node->data < 0) {
            sum += curr_node->data;
        }
        curr_node = curr_node->next;
    }
    cout << "The sum of all negative elements is: " << sum << endl;
}

void displayMax(StackNode *node) {
    int max_elem = node->data;
    StackNode *curr_node = node;
    while (curr_node) {
        max_elem = max(max_elem, curr_node->data);
        curr_node = curr_node->next;
    }
    cout << "Max element is " << max_elem << endl;
}

void sumUptoLastPositive(StackNode *node) {
    int sum = 0;
    int sum_last_positive = 0;
    StackNode *curr_node = node;
    while (curr_node) {
        sum += curr_node->data;
        if (curr_node->data > 0) {
            sum_last_positive = sum;
        }
        curr_node = curr_node->next;
    }
    cout << "The sum of the elements up to the last positive element: " << sum_last_positive
         << endl;
}

int removeIf(StackNode *&node, function<bool(StackNode *, int)> test) {
    StackNode *prev = nullptr;
    StackNode *curr = node;
    int deletedElems = 0;
    int index = 0;
    while (curr) {
        index++;
        if (test(curr, index - 1)) {
            deletedElems++;
            if (prev) {
                prev->next = curr->next;
                StackNode *temp = curr->next;
                delete curr;
                curr = temp;
                continue;
            } else {
                node = node->next;
                StackNode *temp = curr;
                curr = curr->next;
                delete temp;
                continue;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    return deletedElems;
}

int removeEven(StackNode *&node) {
    return removeIf(node, [](StackNode *node, int n) -> bool { return node->data % 2 == 0; });
}

int removeRange(StackNode *&node, int a, int b) {
    return removeIf(node, [a, b](StackNode *node, int n) -> bool {
        return node->data >= a && node->data <= b;
    });
}

int removeN(StackNode *&node, int n) {
    return removeIf(node, [n](StackNode *node, int index) -> bool {
        cout << index;
        return index == n;
    });
}

int insertXifLessThanN(StackNode *node, int check_num, int insert_num) {
    StackNode *curr = node;
    int addedElems = 0;
    while (curr) {
        if (curr->data < check_num) {
            StackNode *curr_temp = curr->next;
            curr->next = add(curr->next, insert_num);
            curr = curr_temp;
            addedElems++;
        } else {
            curr = curr->next;
        }
    }
    return addedElems;
}

void numberOfPositiveNegativePairs(StackNode *node) {
    StackNode *curr = node;
    int pairs = 0;
    while (curr && curr->next) {
        if (curr->data * curr->next->data < 0) {
            pairs++;
        }
        curr = curr->next;
    }
    cout << "Number of positive-negative pairs is " << pairs << endl;
}

int main() {
    StackNode *root = nullptr;
    int n, size = 0, a, b;
    bool manual;
    while (true) {
        if (!root) {
            coutWithColor(colors::LIGHT_RED, "Stack is empty\n");
        } else {
            coutWithColor(colors::LIGHT_BLUE, "--- Stack --- (" + to_string(size) + ")\n");
            coutWithColor(colors::LIGHT_GREEN, "Regular view ---\n");
            view(root);
            coutWithColor(colors::LIGHT_GREEN, "Reverse view (recursive) ---\n");
            view_rec_reverse(root);
            coutWithColor(colors::LIGHT_GREEN, "Recursive view ---\n");
            view_rec(root);
        }
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=MENU=-=-=-=-=-=-=-\n");
        switch (displaySelection(
            {"1.Add elements", "2.Delete n elements from start",
             "3.Delete elements between maximum and minimum elements",
             "4.Delete all even elements", "5.Display sum of all negative elements",
             "6.Display product of elements between maximum and minimum elements",
             "7.Display the sum of the elements up to the last positive element.",
             "8.Display max element", "9.Remove element at index",
             "10.Remove elements with data in interval",
             "11.Insert X after every element less than N",
             "12.Calculate number of positive-negative pairs", "13.Exit"})) {
        case 1:
            n = (int)inputDouble("How many elements to add? : ", false);
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED,
                              "Can't add 0 or negative number of elements\n");
                break;
            }
            size += n;
            coutWithColor(colors::LIGHTER_BLUE, "\nHow would you like to input elements?\n");
            manual = displaySelection({"1.Randomly", "2.Manually"}) == 2;
            for (int i = 0; i < n; i++) {
                if (manual) {
                    root = add(root, inputDouble(""));
                } else {
                    root = add(root, rand() % 100 / 2 - 25);
                }
            }
            clearScreen();
            coutWithColor(colors::LIGHTER_BLUE, "Added " + to_string(n) + " elements\n");
            break;
        case 2:
            if (root) {
                n = (int)inputDouble("How many elements to delete? : ", false);
                clearScreen();
                if (n <= 0) {
                    coutWithColor(colors::LIGHTER_BLUE, "Nothing to delete, n <= 0\n");
                    break;
                }
                del(root, n);
                n = min(n, size);
                size -= n;
                coutWithColor(colors::LIGHTER_BLUE, "Deleted " + to_string(n) + " elements\n");
            } else {
                clearScreen();
                coutWithColor(colors::LIGHTER_BLUE, "Nothing to delete, stack is empty\n");
            }
            break;
        case 3:
            clearScreen();
            if (root) {
                size -= remove_between_min_max(root);
            } else {
                coutWithColor(colors::LIGHTER_BLUE, "Nothing to delete, stack is empty\n");
            }
            break;
        case 4:
            clearScreen();
            if (root) {
                size -= removeEven(root);
            }
            break;
        case 5:
            clearScreen();
            if (root) {
                sumAllNegative(root);
            }
            break;
        case 6:
            clearScreen();
            if (root) {
                product_between_min_max(root);
            }
            break;
        case 7:
            clearScreen();
            if (root) {
                sumUptoLastPositive(root);
            }
            break;
        case 8:
            clearScreen();
            if (root) {
                displayMax(root);
            }
            break;
        case 9:
            n = (int)inputDouble("Index to remove: ", false);
            clearScreen();
            if (n < 0) {
                coutWithColor(colors::LIGHT_RED, "Invalid index!\n");
                break;
            }
            if (root) {
                size -= removeN(root, n);
            }
            break;
        case 10:
            a = (int)inputDouble("Range start: ", false);
            b = (int)inputDouble("Range end: ", false);
            clearScreen();
            if (b < a) {
                coutWithColor(colors::LIGHT_RED, "Invalid range!\n");
                break;
            }
            if (root) {
                size -= removeRange(root, a, b);
            }
            break;
        case 11:
            a = (int)inputDouble("X (number to insert): ", false);
            b = (int)inputDouble("N (number to check and insert after): ", false);
            clearScreen();
            if (root) {
                size += insertXifLessThanN(root, b, a);
            }
            break;
        case 12:
            clearScreen();
            if (root) {
                numberOfPositiveNegativePairs(root);
            }
            break;
        case 13:
            if (root)
                del(root, size);
            return 0;
        }
    }
    return 1;
}
