#include "../genericFunctions.h"
#include <iostream>

using namespace std;

struct TreeNode {
    TreeNode() {
        right = nullptr;
        left = nullptr;
        parent = nullptr;
        data = -100;
    }
    int data;
    TreeNode* right;
    TreeNode* left;
    TreeNode* parent;
};

TreeNode* addElement(int value, TreeNode* root) {
    TreeNode* temp = root;
    TreeNode* elem = new TreeNode;
    elem->data = value;
    
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
                    elem->parent = temp;
                    break;
                }
            } else {
                if (temp->left) {
                    temp = temp->left;
                } else {
                    temp->left = elem;
                    elem->parent = temp;
                    break;
                }
            }
        }
        return root;
    }
    return elem;
}

void printTree(const string& prefix, const TreeNode* node, bool isLeft)
{
    if (node != nullptr)
    {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──");

        // print the value of the node
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

int getHeight(TreeNode* root)
{
    if (root == NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = getHeight(root->left);
        int rheight = getHeight(root->right);

        /* use the larger one */
        return max(lheight, rheight) + 1;
    }
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
        cout << "\n";
        coutWithColor(colors::LIGHT_YELLOW, "-=-=-=-=-=-=-=МЕНЮ=-=-=-=-=-=-=-\n");
        int choise = displaySelection(new string[3]{
            "1.Добавить элементы в дерево",
            "2.-----",
            "3.Выйти" }, 3);
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

        }
    }
    
}
