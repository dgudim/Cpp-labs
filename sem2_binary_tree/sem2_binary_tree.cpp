#include "../genericFunctions.h"
#include <iostream>
#include <math.h>

using namespace std;

uint32_t hash_string(string str);

struct TreeNode {
    TreeNode(string k = "", string dat = "") {
        right = nullptr;
        left = nullptr;
        key_str = k;
        key_hash = hash_string(key_str);
        data = dat;
    }
    uint32_t key_hash;
    string key_str;
    string data;
    TreeNode *right;
    TreeNode *left;
};

TreeNode *findByKey(TreeNode *root, uint32_t key);

string inputKey(string message) {
    return inputString(message,
                       "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890");
}

TreeNode *addElement(TreeNode *root, bool &success) {

    string key = inputKey("Key: ");

    if (findByKey(root, hash_string(key))) {
        coutWithColor(colors::LIGHT_RED, "Duplicate key, please try again\n");
        success = false;
        return root;
    }

    TreeNode *temp = root;
    TreeNode *elem = new TreeNode(
        key, inputString("Data: ",
                         "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890"));
    success = true;

    if (root) {
        while (temp) {
            if (elem->key_hash > temp->key_hash) {
                if (temp->right) {
                    temp = temp->right;
                } else {
                    temp->right = elem;
                    break;
                }
            } else {
                if (temp->left) {
                    temp = temp->left;
                } else {
                    temp->left = elem;
                    break;
                }
            }
        }
        return root;
    }
    return elem;
}

TreeNode *getMinNode(TreeNode *node) {
    if (!node)
        return node;
    TreeNode *current = node;

    while (current && current->left)
        current = current->left;

    return current;
}

TreeNode *deleteByKey(TreeNode *root, uint32_t key) {

    if (!root)
        return root;

    if (key < root->key_hash)
        root->left = deleteByKey(root->left, key);

    else if (key > root->key_hash)
        root->right = deleteByKey(root->right, key);

    else {

        if (!root->left && !root->right)
            return nullptr;

        else if (!root->left) {
            TreeNode *temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            TreeNode *temp = root->left;
            delete root;
            return temp;
        }

        TreeNode *temp = getMinNode(root->right);

        root->key_hash = temp->key_hash;
        root->key_str = temp->key_str;

        root->right = deleteByKey(root->right, temp->key_hash);
    }
    return root;
}

void deleteWholeTree(TreeNode *root) {
    if (root) {
        deleteWholeTree(root->right);
        deleteWholeTree(root->left);
        delete root;
    }
}

void printTreeHorizontal(const string &prefix, const TreeNode *node, bool isLeft) {
    if (node) {
        coutWithColor(colors::LIGHTER_BLUE, prefix);
        coutWithColor(colors::LIGHT_BLUE, isLeft ? "├──" : "└──");

        coutWithColor(colors::LIGHT_YELLOW, node->key_str + "\n");

        printTreeHorizontal(prefix + (isLeft ? "│   " : "    "), node->left, true);
        printTreeHorizontal(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printTreeHorizontal(TreeNode *root) {
    if (!root) {
        coutWithColor(colors::LIGHT_RED, "Tree is empty\n");
        return;
    }
    printTreeHorizontal("", root, false);
}

int countLeaves(TreeNode *root) {
    if (root) {
        if (!root->left && !root->right) {
            return 1;
        }
        return countLeaves(root->left) + countLeaves(root->right);
    }
    return 0;
}

int countNodes(TreeNode *root) {
    if (!root)
        return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}

void inorderLeftRootRight_infix(TreeNode *root) {
    if (root) {
        inorderLeftRootRight_infix(root->left);
        cout << root->key_str << " | ";
        inorderLeftRootRight_infix(root->right);
    }
}

void inorderRootLeftRight_prefix(TreeNode *root) {
    if (root) {
        cout << root->key_str << " | ";
        inorderRootLeftRight_prefix(root->left);
        inorderRootLeftRight_prefix(root->right);
    }
}

void inorderLeftRightRoot_postfix(TreeNode *root) {
    if (root) {
        inorderLeftRightRoot_postfix(root->left);
        inorderLeftRightRoot_postfix(root->right);
        cout << root->key_str << " | ";
    }
}

int getHeight(TreeNode *root) {
    if (!root)
        return 0;
    return max(getHeight(root->left), getHeight(root->right)) + 1;
}

TreeNode *findByKey(TreeNode *root, uint32_t key) {
    if (!root)
        return nullptr;
    if (root->key_hash == key)
        return root;
    return findByKey(key < root->key_hash ? root->left : root->right, key);
}

TreeNode *findByKey(TreeNode *root, TreeNode *&parent, uint32_t key) {
    if (!root)
        return nullptr;
    if (root->key_hash == key)
        return root;
    parent = root;
    return findByKey(key < root->key_hash ? root->left : root->right, parent, key);
}

void printByKey(TreeNode *root, string key) {
    TreeNode *r = nullptr;
    TreeNode *node = findByKey(root, r, hash_string(key));
    clearScreen();
    if (node) {
        coutWithColor(colors::LIGHTER_BLUE,
                      "Info from the key " + key + ": " + node->data +
                          " (parent: " + (r ? r->key_str : string("null")) + ")\n");
        return;
    }
    coutWithColor(colors::LIGHT_RED, "The tree doesn't have this key\n");
}

void printOldTreeHorizontal(TreeNode *root) {
    clearScreen();
    coutWithColor(colors::LIGHT_BLUE, "Old tree\n");
    printTreeHorizontal(root);
    cout << "\n";
}

void treeToVector(TreeNode *root, vector<TreeNode *> &nodes) {
    if (!root)
        return;

    // в порядке возрастания ключа
    treeToVector(root->left, nodes);
    nodes.push_back(root);
    treeToVector(root->right, nodes);
}

TreeNode *vectorToBalancedTree(vector<TreeNode *> &nodes, int start, int end) {

    if (start > end)
        return nullptr;

    int mid = (start + end) / 2;
    TreeNode *root = nodes[mid];
    // middle element in array in our root

    root->left = vectorToBalancedTree(nodes, start, mid - 1);
    root->right = vectorToBalancedTree(nodes, mid + 1, end);

    return root;
}

TreeNode *balanceTree(TreeNode *root) {
    vector<TreeNode *> nodes;
    treeToVector(root, nodes);

    return vectorToBalancedTree(nodes, 0, nodes.size() - 1);
}

bool compareNodes(TreeNode *n1, TreeNode *n2) { return n1->key_hash < n2->key_hash; }

TreeNode *generateSampleTree(int layers) {
    int nodes_count = (1 << layers) - 1; // 1 << layers = pow(2, layers)
    vector<TreeNode *> nodes;
    for (int i = 0; i < nodes_count; i++) {
        nodes.push_back(new TreeNode("key" + to_string(i), "data" + to_string(i)));
    }
    sort(nodes.begin(), nodes.end(), compareNodes);
    return vectorToBalancedTree(nodes, 0, nodes.size() - 1);
}

constexpr int delay = 7;
colors trunk_color = colors::BLUE;

void printNode(int x, int y, TreeNode *node, int layer, int width, bool animate) {
    int delay_ = animate ? delay : 0;
    if (!node) {
        coutWithColorAtPos(colors::RED, "◈", x, y, delay_);
        return;
    }
    string data_to_print = node->key_str;
    if (data_to_print.length() > 6) {
        data_to_print = data_to_print.substr(0, 6) + "-";
    }
    int num_offset = (data_to_print.length() - 1) / 2;
    coutWithColorAtPos(colors::LIGHT_YELLOW, data_to_print, x - num_offset, y, delay_);
    if (layer >= 0) {
        coutWithColorAtPos(trunk_color, "┴", x, y + 1, delay_);
        for (int i = 1; i < width; i++) {
            coutWithColorAtPos(trunk_color, "─", x + i, y + 1, delay_);
            coutWithColorAtPos(trunk_color, "─", x - i, y + 1, delay_);
        }
        coutWithColorAtPos(trunk_color, "╮", x + width, y + 1, delay_);
        coutWithColorAtPos(trunk_color, "╭", x - width, y + 1, delay_);
        printNode(x + width, y + 2, node->right, layer + 1, width / 2, animate);
        printNode(x - width, y + 2, node->left, layer + 1, width / 2, animate);
    }
}

void printTreeVertical(TreeNode *root, bool animate = false) {
    if (!root) {
        coutWithColor(colors::LIGHT_RED, "Tree is empty\n");
        return;
    }
    int layers = getHeight(root);

    COORD size = getConsoleDimensions();
    COORD pos = getConsoleCursorPosition();

    printNode(size.X / 2, pos.Y + 1, root, 0, 1 << layers, animate);
    setConsoleCursorPosition(0, pos.Y + layers * 2 + 1);
}

void printOldTreeVertical(TreeNode *root) {
    clearScreen();
    coutWithColor(colors::LIGHT_BLUE, "Old tree\n");
    printTreeVertical(root);
    cout << "\n";
}

int main() {
    TreeNode *root = nullptr;
    TreeNode *temp = nullptr;
    TreeNode *temp2 = nullptr;

    TreeNode *min = nullptr;
    TreeNode *max = nullptr;

    int n = 0;
    int n2 = 0;
    bool success = true;
    bool animateOnNextPrint = false;
    bool printInfoOnNextPrint = false;

    int y_offset = 0;

    while (true) {
        printTreeVertical(root, animateOnNextPrint);
        animateOnNextPrint = false;

        if (printInfoOnNextPrint && root) {
            min = root;
            max = root;
            while (min->left) {
                min = min->left;
            }

            while (max->right) {
                max = max->right;
            }
            y_offset = getHeight(root) * 2 + 2;
            coutWithColorAtPos(colors::LIGHT_BLUE, "Info about the tree", 70, y_offset + 1);
            coutWithColorAtPos(colors::LIGHT_GREEN,
                               "Max element: " + max->key_str + " (" +
                                   to_string(max->key_hash) + ")",
                               70, y_offset + 2);
            coutWithColorAtPos(colors::LIGHT_GREEN,
                               "Min element: " + min->key_str + " (" +
                                   to_string(min->key_hash) + ")",
                               70, y_offset + 3);
            coutWithColorAtPos(colors::LIGHT_GREEN,
                               "Tree depth: " + to_string(getHeight(root)), 70, y_offset + 4);
            coutWithColorAtPos(colors::LIGHT_GREEN,
                               "Leaf count: " + to_string(countLeaves(root)), 70,
                               y_offset + 5);
            coutWithColorAtPos(colors::LIGHT_GREEN,
                               "Element count: " + to_string(countNodes(root)), 70,
                               y_offset + 6);
            setConsoleCursorPosition(70, y_offset);
        }
        printInfoOnNextPrint = false;

        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(
            {"1.Add elements to the tree", "2.Delete element by key",
             "3.Delete a branch with a vertex by key", "4.Delete the whole tree",
             "5.Find information by key", "6.Balance the tree",
             "7.Generate a tree with n layers", "8.Info about the tree", "9.Exit"});
        switch (choise) {
        case 1:
            n = (int)inputDouble("How many elements to add? : ", false);
            n2 = n;
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED,
                              "Cannot add 0 or negative number of elements\n");
                break;
            }

            printOldTreeVertical(root);

            coutWithColor(colors::LIGHT_BLUE, "Input elements(" + to_string(n) + "): ");

            for (int i = 0; i < n; i++) {
                root = addElement(root, success);
                n += !success;
            }

            coutWithColor(colors::LIGHTER_BLUE, "Added " + to_string(n2) + " element(s)\n");
            break;
        case 2:

            printOldTreeVertical(root);

            root = deleteByKey(root, hash_string(inputKey("Input the key for deletion: ")));
            break;
        case 3:

            printOldTreeVertical(root);

            temp =
                findByKey(root, temp2, hash_string(inputKey("Input the key for deletion: ")));

            if (!temp) {
                coutWithColor(colors::LIGHT_RED, "There is no such key\n");
                break;
            }
            deleteWholeTree(temp);
            if (temp == root) {
                root = nullptr;
            } else {
                if (temp2->right == temp) {
                    temp2->right = nullptr;
                } else {
                    temp2->left = nullptr;
                }
            }

            break;
        case 4:
            deleteWholeTree(root);
            root = nullptr;
            clearScreen();
            break;
        case 5:
            printByKey(root, inputKey("Key: "));
            break;
        case 6:
            clearScreen();
            coutWithColor(colors::LIGHTER_BLUE, "Balanced the tree\n");
            root = balanceTree(root);
            break;
        case 7:
            n = (int)inputDouble("How many layers? : ", false);
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED,
                              "Cannot create 0 or negative number of layers\n");
                break;
            }
            root = generateSampleTree(n);
            animateOnNextPrint = true;
            clearScreen();
            break;
        case 8:
            clearScreen();
            printInfoOnNextPrint = true;
            break;
        case 9:
            return 0;
        }
    }
}

uint32_t hash_string(string str) {
    uint32_t hash = 3323198485ul;
    for (char ch : str) {
        hash ^= ch;
        hash *= 0x5bd1e995;
        hash ^= hash >> 15;
    }
    return hash;
}
