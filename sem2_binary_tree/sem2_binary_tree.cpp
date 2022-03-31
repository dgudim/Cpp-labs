#include "../genericFunctions.h"
#include <iostream>
#include <math.h>

using namespace std;

struct TreeNode {
    TreeNode(int k = -100, string dat = "") {
        right = nullptr;
        left = nullptr;
        key = k;
        data = dat;
    }
    int key;
    string data;
    TreeNode* right;
    TreeNode* left;
};

TreeNode* findByKey(TreeNode* root, int key);

TreeNode* addElement(TreeNode* root, bool& success) {

    int key = (int)inputData("Ключ: ");

    if (findByKey(root, key)) {
        coutWithColor(colors::LIGHT_RED, "Дубликат ключа, повторите ввод\n");
        success = false;
        return root;
    }

    TreeNode* temp = root;
    TreeNode* elem = new TreeNode(key, inputData("Данные: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64));
    success = true;

    if (root) {
        while (temp) {
            if (elem->key > temp->key) {
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

TreeNode* getMinNode(TreeNode* node) {
    if (!node) return node;
    TreeNode* current = node;

    while (current && current->left)
        current = current->left;

    return current;
}

TreeNode* deleteByKey(TreeNode* root, int key) {

    if (!root) return root;

    if (key < root->key)
        root->left = deleteByKey(root->left, key);

    else if (key > root->key)
        root->right = deleteByKey(root->right, key);

    else {

        if (!root->left && !root->right)
            return nullptr;

        else if (!root->left) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }

        TreeNode* temp = getMinNode(root->right);

        root->key = temp->key;

        root->right = deleteByKey(root->right, temp->key);
    }
    return root;
}

void deleteWholeTree(TreeNode* root) {
    if (root) {
        deleteWholeTree(root->right);
        deleteWholeTree(root->left);
        delete root;
    }
}

void printTreeHorizontal(const string& prefix, const TreeNode* node, bool isLeft)
{
    if (node) {
        coutWithColor(colors::LIGHTER_BLUE, prefix);
        coutWithColor(colors::LIGHT_BLUE, isLeft ? "├──" : "└──");

        coutWithColor(colors::LIGHT_YELLOW, to_string(node->key) + "\n");

        printTreeHorizontal(prefix + (isLeft ? "│   " : "    "), node->left, true);
        printTreeHorizontal(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printTreeHorizontal(TreeNode* root)
{
    if (!root) {
        coutWithColor(colors::LIGHT_RED, "Дерево пустое\n");
        return;
    }
    printTreeHorizontal("", root, false);
}

int countLeaves(TreeNode* root) {
    if (root) {
        if (!root->left && !root->right) {
            return 1;
        }
        return countLeaves(root->left) + countLeaves(root->right);
    }
    return 0;
}

int countNodes(TreeNode* root) {
    if (!root) return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}

void inorderLeftRootRight_infix(TreeNode* root)
{
    if (root) {
        inorderLeftRootRight_infix(root->left);
        cout << root->key << " | ";
        inorderLeftRootRight_infix(root->right);
    }
}

void inorderRootLeftRight_prefix(TreeNode* root)
{
    if (root) {
        cout << root->key << " | ";
        inorderRootLeftRight_prefix(root->left);
        inorderRootLeftRight_prefix(root->right);
    }
}

void inorderLeftRightRoot_postfix(TreeNode* root)
{
    if (root) {
        inorderLeftRightRoot_postfix(root->left);
        inorderLeftRightRoot_postfix(root->right);
        cout << root->key << " | ";
    }
}

int getHeight(TreeNode* root)
{
    if (!root) return 0;
    return max(getHeight(root->left), getHeight(root->right)) + 1;
}

TreeNode* findByKey(TreeNode* root, int key) {
    if (!root) return nullptr;
    if (root->key == key) return root;
    return findByKey(key < root->key ? root->left : root->right, key);
}

TreeNode* findByKey(TreeNode* root, TreeNode*& parent, int key) {
    if (!root) return nullptr;
    if (root->key == key) return root;
    parent = root;
    return findByKey(key < root->key ? root->left : root->right, parent, key);
}

void printByKey(TreeNode* root, int key) {
    TreeNode* r = nullptr;
    TreeNode* node = findByKey(root, r, key);
    clearScreen();
    if (node) {
        coutWithColor(colors::LIGHTER_BLUE, "Информация по ключу " + to_string(key) + ": " + node->data + " (родитель: " + (r ? to_string(r->key) : string("null")) + ")\n");
        return;
    }
    coutWithColor(colors::LIGHT_RED, "В дереве нет такого ключа\n");
}

void printOldTreeHorizontal(TreeNode* root) {
    clearScreen();
    coutWithColor(colors::LIGHT_BLUE, "Старое дерево\n");
    printTreeHorizontal(root);
    cout << "\n";
}

void treeToVector(TreeNode* root, vector<TreeNode*>& nodes) {
    if (!root) return;

    // в порядке возрастания ключа
    treeToVector(root->left, nodes);
    nodes.push_back(root);
    treeToVector(root->right, nodes);
}

TreeNode* vectorToBalancedTree(vector<TreeNode*>& nodes, int start, int end) {

    if (start > end) return nullptr;

    int mid = (start + end) / 2;
    TreeNode* root = nodes[mid];
    //middle element in array in our root

    root->left = vectorToBalancedTree(nodes, start, mid - 1);
    root->right = vectorToBalancedTree(nodes, mid + 1, end);

    return root;
}

TreeNode* balanceTree(TreeNode* root)
{
    vector<TreeNode*> nodes;
    treeToVector(root, nodes);

    return vectorToBalancedTree(nodes, 0, nodes.size() - 1);
}

TreeNode* generateSampleTree(int layers) {
    int nodes_count = (1 << layers) - 1; // 1 << layers = pow(2, layers)
    vector<TreeNode*> nodes;
    for (int i = 0; i < nodes_count; i++) {
        nodes.push_back(new TreeNode(i, to_string(i)));
    }
    return vectorToBalancedTree(nodes, 0, nodes.size() - 1);
}

void printNode(int x, int y, TreeNode* node, int layer, int layers) {
    setConsoleCursorPosition(x, y);
    if (!node) {
        coutWithColor(colors::PURPLE, "N");
        return;
    }
    coutWithColor(colors::LIGHT_YELLOW, node->data);
    if (layer < layers) {
        int width = (layers - layer) * 3 - 1;
        setConsoleCursorPosition(x, y + 1);
        coutWithColor(colors::BLUE, "|");
        for (int i = 1; i < width; i++) {
            setConsoleCursorPosition(x + i, y + 1);
            coutWithColor(colors::BLUE, "⎻");
            setConsoleCursorPosition(x - i, y + 1);
            coutWithColor(colors::BLUE, "⎻");
        }
        setConsoleCursorPosition(x + width, y + 1);
        coutWithColor(colors::LIGHTER_BLUE, "⎤");
        setConsoleCursorPosition(x - width, y + 1);
        coutWithColor(colors::LIGHTER_BLUE, "⎡");
        printNode(x + width, y + 2, node->right, layer + 1, layers);
        printNode(x - width, y + 2, node->left, layer + 1, layers);
    }
}

void printTreeVertical(TreeNode* root) {
    int layers = getHeight(root);
    int width = 0;
    COORD pos = getConsoleCursorPosition();
    for (int i = 0; i < layers; i++) {
        width += (layers - i) * 3;
    }
    printNode(width + 1, pos.Y + 1, root, 0, layers);
    setConsoleCursorPosition(0, pos.Y + layers * 3);
}

int main()
{
    TreeNode* root = nullptr;
    TreeNode* temp = nullptr;
    TreeNode* temp2 = nullptr;

    TreeNode* min = nullptr;
    TreeNode* max = nullptr;

    int n = 0;
    int n2 = 0;
    bool success = true;

    while (true) {
        printTreeVertical(root);

        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[9]{
            "1.Добавить элементы в дерево",
            "2.Удалить элемент по ключу",
            "3.Удалить ветвь с вершиной по ключу",
            "4.Удалить все дерево",
            "5.Найти информацию по ключу",
            "6.Сбалансировать дерево",
            "7.Сгенерировать дерево по количеству слоев",
            "8.Информация о дереве",
            "9.Выйти" }, 9);
        switch (choise) {
        case 1:
            n = (int)inputData("Сколько элементов добавить? : ", false);
            n2 = n;
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество элементов\n");
                break;
            }

            printOldTreeHorizontal(root);

            coutWithColor(colors::LIGHT_BLUE, "Введите элементы(" + to_string(n) + "): ");

            for (int i = 0; i < n; i++) {
                root = addElement(root, success);
                n += !success;
            }

            coutWithColor(colors::LIGHTER_BLUE, "Добавил " + to_string(n2) + " элемента(ов)\n");
            break;
        case 2:

            printOldTreeHorizontal(root);

            root = deleteByKey(root, (int)inputData("Введите ключ для удаления: "));
            break;
        case 3:

            printOldTreeHorizontal(root);

            temp = findByKey(root, temp2, (int)inputData("Введите ключ для удаления: "));

            if (!temp) {
                coutWithColor(colors::LIGHT_RED, "В дереве нет такого ключа\n");
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
            printByKey(root, (int)inputData("Ключ: "));
            break;
        case 6:
            clearScreen();
            coutWithColor(colors::LIGHTER_BLUE, "Сбалансировал дерево\n");
            root = balanceTree(root);
            break;
        case 7:
            n = (int)inputData("Сколько слоев? : ", false);
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество слоев\n");
                break;
            }
            root = generateSampleTree(n);
            clearScreen();
            break;
        case 8:
            clearScreen();
            if (root) {
                min = root;
                max = root;
                while (min->left) {
                    min = min->left;
                }
                while (max->right) {
                    max = max->right;
                }

                setConsoleCursorPosition(80, 1);
                coutWithColor(colors::LIGHT_BLUE, "Информация по дереву");
                setConsoleCursorPosition(80, 2);
                coutWithColor(colors::LIGHT_GREEN, "Максимальный элемент: " + to_string(max->key));
                setConsoleCursorPosition(80, 3);
                coutWithColor(colors::LIGHT_GREEN, "Минимальный элемент: " + to_string(min->key));
                setConsoleCursorPosition(80, 4);
                coutWithColor(colors::LIGHT_GREEN, "Глубина дерева: " + to_string(getHeight(root)));
                setConsoleCursorPosition(80, 5);
                coutWithColor(colors::LIGHT_GREEN, "Количество листьев: " + to_string(countLeaves(root)));
                setConsoleCursorPosition(80, 6);
                coutWithColor(colors::LIGHT_GREEN, "Количество элементов: " + to_string(countNodes(root)));
                setConsoleCursorPosition(80, 7);
                inorderLeftRootRight_infix(root);
                setConsoleCursorPosition(80, 8);
                inorderRootLeftRight_prefix(root);
                setConsoleCursorPosition(80, 9);
                inorderLeftRightRoot_postfix(root);

                setConsoleCursorPosition(0, 0);
            }
            break;
        case 9:
            return 0;
        }
    }

}
