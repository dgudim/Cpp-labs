#include "../genericFunctions.h"
#include <iostream>

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

TreeNode* addElement(string data, int key, TreeNode* root, bool& success) {
    TreeNode* temp = root;
    TreeNode* elem = new TreeNode(key, data);
    success = true;

    if (root) {
        while (temp) {
            if (elem->key == temp->key) {
                coutWithColor(colors::LIGHT_RED, "Дубликат ключа, повторите ввод\n");
                success = false;
                break;
            }
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

void printTree(const string& prefix, const TreeNode* node, bool isLeft)
{
    if (node != nullptr)
    {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──");

        cout << node->key << endl;

        printTree(prefix + (isLeft ? "│   " : "    "), node->left, true);
        printTree(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printMinMax(TreeNode* root) {
    TreeNode* min = root;
    TreeNode* max = root;
    while (min->left) {
        min = min->left;
    }
    while (max->right) {
        max = max->right;
    }
    coutWithColor(colors::LIGHT_GREEN, "Максимальный элемент: " + to_string(max->key) + "\n");
    coutWithColor(colors::LIGHT_GREEN, "Минимальный элемент: " + to_string(min->key) + "\n");
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

void printTree(TreeNode* root)
{
    if (!root) {
        coutWithColor(colors::LIGHT_RED, "Дерево пустое\n");
        return;
    }
    printTree("", root, false);
    printMinMax(root);
    coutWithColor(colors::LIGHT_GREEN, "Глубина дерева: " + to_string(getHeight(root)) + "\n");
    inorderLeftRootRight_infix(root);
    cout << "\n";
    inorderRootLeftRight_prefix(root);
    cout << "\n";
    inorderLeftRightRoot_postfix(root);
    cout << "\n";
}

TreeNode* findByKey(TreeNode* root, TreeNode*& parent, int key) {
    if (!root) return nullptr;

    if (key < root->key) {
        parent = root;
        return findByKey(root->left, parent, key);
    }

    if (key > root->key) {
        parent = root;
        return findByKey(root->right, parent, key);
    }

    return root;
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

void printOldTree(TreeNode* root) {
    clearScreen();
    coutWithColor(colors::LIGHT_BLUE, "Старое дерево\n");
    printTree(root);
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

    int n = nodes.size();
    return vectorToBalancedTree(nodes, 0, n - 1);
}

int main()
{
    TreeNode* root = nullptr;
    TreeNode* temp = nullptr;
    TreeNode* temp2 = nullptr;

    int n = 0;
    int n2 = 0;
    bool success = true;

    while (true) {
        printTree(root);
        coutWithColor(colors::LIGHT_YELLOW, "\n-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[7]{
            "1.Добавить элементы в дерево",
            "2.Удалить элемент по ключу",
            "3.Удалить ветвь с вершиной по ключу",
            "4.Удалить все дерево",
            "5.Найти информацию по ключу",
            "6.Сбалансировать дерево",
            "7.Выйти" }, 7);
        switch (choise) {
        case 1:
            n = (int)inputData("Сколько элементов добавить? : ", false);
            n2 = n;
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество элементов\n");
                break;
            }

            printOldTree(root);

            coutWithColor(colors::LIGHT_BLUE, "Введите элементы(" + to_string(n) + "): ");

            for (int i = 0; i < n; i++) {
                root = addElement(inputData("Данные: ", new char[65]{ "qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM._1234567890" }, 64), (int)inputData("Ключ: "), root, success);
                n += !success;
            }

            coutWithColor(colors::LIGHTER_BLUE, "Добавил " + to_string(n2) + " элементов\n");
            break;
        case 2:

            printOldTree(root);

            root = deleteByKey(root, (int)inputData("Введите ключ для удаления: "));
            break;
        case 3:

            printOldTree(root);

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
            return 0;
        }
    }

}
