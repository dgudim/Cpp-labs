#include "../genericFunctions.h"
#include <iostream>
using namespace std;

struct QueueNode {
    string data;
    QueueNode *next;
    QueueNode *prev;

    uint32_t hash() {
        uint32_t hash = 3323198485ul;
        for (char ch : data) {
            hash ^= ch;
            hash *= 0x5bd1e995;
            hash ^= hash >> 15;
        }
        return hash;
    }
};

void add(QueueNode *&root, QueueNode *&tail, string data, bool back) {
    QueueNode *node = new QueueNode;
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

void view(QueueNode *end_node) {
    QueueNode *curr_node = end_node;
    bool from_back = !curr_node->next;
    while (curr_node) {
        cout << curr_node->data << " ";
        curr_node = from_back ? curr_node->prev : curr_node->next;
    }
    cout << endl;
}

void del(QueueNode *&root, QueueNode *&tail, int n, bool from_back) {
    QueueNode *p = from_back ? tail : root;
    QueueNode *curr_node = p;
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
        } else {
            root = p;
            root->prev = nullptr;
        }
    } else {
        tail = nullptr;
        root = nullptr;
    }
}

void printQueue(QueueNode *&root, QueueNode *&tail, int size) {
    if (!root) {
        coutWithColor(colors::LIGHT_BLUE, "--- Queue --- (" + to_string(size) + ")\n");
        coutWithColor(colors::LIGHT_RED, "Queue is empty\n");
    } else {
        coutWithColor(colors::LIGHT_BLUE, "--- Queue --- (" + to_string(size) + ")\n");
        coutWithColor(colors::LIGHT_GREEN, "Regular output ---\n");
        view(root);
        coutWithColor(colors::LIGHT_GREEN, "Reverse ---\n");
        view(tail);
    }
}

int remove_between_min_max(QueueNode *&root, QueueNode *&tail, int size) {
    QueueNode *min = root;
    QueueNode *max = root;
    QueueNode *curr_node = root;
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
        coutWithColor(colors::LIGHT_YELLOW,
                      "Minimum and maximum elements are adjacent, nothing to remove\n");
        return 0;
    }

    coutWithColor(colors::BLUE, "Old queue\n");
    printQueue(root, tail, size);
    cout << "\n";

    if (!minFirst) {
        QueueNode *temp = min;
        min = max;
        max = temp;
    }
    int deletedElems = 0;
    QueueNode *next_node = min->next;
    min->next = max;
    max->prev = min;
    while (next_node != max) {
        deletedElems++;
        curr_node = next_node;
        next_node = curr_node->next;
        delete curr_node;
    }
    coutWithColor(colors::LIGHT_YELLOW,
                  "Deleted " + to_string(deletedElems) + " element(s) between max(" +
                      (minFirst ? max->data : min->data) + ") and min(" +
                      (minFirst ? min->data : max->data) + ") elements\n");
    return deletedElems;
}

int deleteAllNegative(QueueNode *&root, QueueNode *&tail) {
    int deletedElems = 0;
    QueueNode *curr_node = root;
    bool delete_ = false;
    while (curr_node) {
        if (curr_node->hash() < 0) {
            if (curr_node->next) {
                curr_node->next->prev = curr_node->prev;
            }
            if (curr_node->prev) {
                curr_node->prev->next = curr_node->next;
            }
            delete_ = true;
            deletedElems++;
        }
        if (delete_) {
            QueueNode *temp_node = curr_node;
            if (curr_node == root && root) {
                root = root->next;
            }
            if (curr_node == tail && tail) {
                tail = tail->prev;
            }
            curr_node = curr_node->next;
            delete temp_node;
            delete_ = false;
        } else {
            curr_node = curr_node->next;
        }
    }
    coutWithColor(colors::LIGHT_YELLOW,
                  "Deleted " + to_string(deletedElems) + " element(s)\n");
    return deletedElems;
}

int main() {
    QueueNode *root = nullptr;
    QueueNode *tail = nullptr;
    int n, size = 0, subs = 0;
    bool manual;
    while (true) {
        printQueue(root, tail, size);
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=MENU=-=-=-=-=-=-=-\n");
        int choise = displaySelection({"1.Add elements to the queue from the beginning",
                                       "2.Add elements to the queue from the end",
                                       "3.Delete n elements from the beginning",
                                       "4.Delete n elements from the end",
                                       "5.Delete elements between min and max elements",
                                       "6.Delete all negative elements", "7.Exit"});
        switch (choise) {
        case 1:
        case 2:
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

            clearScreen();
            coutWithColor(colors::BLUE, "Old queue\n");
            printQueue(root, tail, size);
            cout << "\n";

            if (manual) {
                coutWithColor(colors::BLUE, "Input elements(" + to_string(n) + "): ");
            }

            for (int i = 0; i < n; i++) {
                if (manual) {
                    add(root, tail, inputString("", "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890"), choise == 2);
                } else {
                    add(root, tail, to_string(rand() % 100 / 2 - 25), choise == 2);
                }
            }

            coutWithColor(colors::LIGHTER_BLUE,
                          "Added " + to_string(n) + " elements from " +
                              (choise == 2 ? "the end\n" : "the beginning\n"));
            break;
        case 3:
        case 4:
            if (root) {
                n = (int)inputDouble("How many elements to delete? : ", false);
                clearScreen();
                if (n <= 0) {
                    coutWithColor(colors::LIGHTER_BLUE,
                                  "There's nothing to delete, you entered a number <= 0\n");
                    break;
                }
                coutWithColor(colors::BLUE, "Old queue\n");
                printQueue(root, tail, size);
                cout << "\n";
                del(root, tail, n, choise == 4);
                n = min(n, size);
                size -= n;
                coutWithColor(colors::LIGHTER_BLUE,
                              "Deleted " + to_string(n) + " elements from " +
                                  (choise == 4 ? "the end\n" : "the beginning\n"));
            } else {
                clearScreen();
                coutWithColor(colors::LIGHTER_BLUE,
                              "There's nothing to delete, the queue is empty\n");
            }
            break;
        case 5:
            clearScreen();
            if (root) {
                coutWithColor(colors::BLUE, "Old queue\n");
                printQueue(root, tail, size);
                cout << "\n";
                size -= remove_between_min_max(root, tail, size);
            } else {
                coutWithColor(colors::LIGHTER_BLUE,
                              "There's nothing to delete, the queue is empty\n");
            }
            break;
        case 6:
            clearScreen();
            if (root) {
                coutWithColor(colors::BLUE, "Old queue\n");
                printQueue(root, tail, size);
                cout << "\n";
                size -= deleteAllNegative(root, tail);
            } else {
                coutWithColor(colors::LIGHTER_BLUE,
                              "There's nothing to delete, the queue is empty\n");
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
