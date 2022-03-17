#include "../genericFunctions.h"
#include <iostream>

using namespace std;

struct TreeNode {
    TreeNode(int dat = -100) {
        right = nullptr;
        left = nullptr;
        data = dat;
    }
    int data;
    TreeNode* right;
    TreeNode* left;
};

TreeNode* addElement(int value, TreeNode* root) {
    TreeNode* temp = root;
    TreeNode* elem = new TreeNode(value);

    if (root) {
        while (temp) {
            if (elem->data == temp->data) {
                coutWithColor(colors::LIGHT_RED, "Дубликат ключа\n");
                break;
            }
            if (elem->data > temp->data) {
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

    if (key < root->data)
        root->left = deleteByKey(root->left, key);

    else if (key > root->data)
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

        root->data = temp->data;

        root->right = deleteByKey(root->right, temp->data);
    }
    return root;
}

void deleteWholeTree(TreeNode* root) {
    if(root){
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

        cout << node->data << endl;

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
    coutWithColor(colors::LIGHT_GREEN, "Максимальный элемент: " + to_string(max->data) + "\n");
    coutWithColor(colors::LIGHT_GREEN, "Минимальный элемент: " + to_string(min->data) + "\n");
}

void inorder(TreeNode* root)
{
    if (root != NULL) {
        inorder(root->left);
        cout << root->data << " | ";
        inorder(root->right);
    }
}

int getHeight(TreeNode* root)
{
    if (!root) return 0;
    return max(getHeight(root->left), getHeight(root->right)) + 1;
}

void printLeftRootRight_infix(TreeNode* root) {

}

void printRootLeftRight_prefix(TreeNode* root) {

}

void printLeftRightRoot_postfix(TreeNode* root) {

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
}

int main()
{
    TreeNode* root = nullptr;

    int n = 0;

    while (true) {
        printTree(root);
        inorder(root);
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[4]{
            "1.Добавить элементы в дерево",
            "2.Удалить элемент по ключу",
            "3.Удалить все дерево",
            "4.Выйти" }, 4);
        switch (choise) {
        case 1:
            n = (int)inputData("Сколько элементов добавить? : ", false);
            if (n <= 0) {
                clearScreen();
                coutWithColor(colors::LIGHT_RED, "Не могу добавить 0 или отрицательное количество элементов\n");
                break;
            }

            clearScreen();
            coutWithColor(colors::LIGHT_BLUE, "Старое дерево\n");
            printTree(root);
            cout << "\n";

            coutWithColor(colors::LIGHT_BLUE, "Введите элементы(" + to_string(n) + "): ");

            for (int i = 0; i < n; i++) {
                root = addElement((int)inputData(""), root);
            }

            coutWithColor(colors::LIGHTER_BLUE, "Добавил " + to_string(n) + " элементов\n");
            break;
        case 2:

            clearScreen();
            coutWithColor(colors::LIGHT_BLUE, "Старое дерево\n");
            printTree(root);
            cout << "\n";

            root = deleteByKey(root, (int)inputData("Введите ключ для удаления: "));
            break;
        case 3:
            deleteWholeTree(root);
            root = nullptr;
            clearScreen();
            break;
        case 4:
            return 0;
        }
    }

}
