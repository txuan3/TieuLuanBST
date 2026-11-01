#include <iostream>
using namespace std;

struct Node
{
    int key;
    Node* left;
    Node* right;
};

Node* createNode(int x)
{
    Node* p = new Node;
    p->key = x;
    p->left = nullptr;
    p->right = nullptr;
    return p;
}
void addNode(Node*& root, const int x)
{
    if (root == nullptr)
        root = createNode(x);
    else if (root->key > x)
        addNode(root->left, x);
    else if (root->key < x)
        addNode(root->right, x);
}
Node* searchNode(Node* root, const int x)
{
    if (root == nullptr || root->key == x)
        return root;
    else if (root->key > x)
        return searchNode(root->left, x);
    else
        return searchNode(root->right, x);
}
void deleteNode(Node*& root, const int x)
{
    if (root == nullptr)
        return;

    if (x < root->key)
        deleteNode(root->left, x);
    else if (x > root->key)
        deleteNode(root->right, x);
    else
    {
        if (root->left == nullptr)
        {
            Node* temp = root;
            root = root->right;
            delete temp;
        }
        else if (root->right == nullptr)
        {
            Node* temp = root;
            root = root->left;
            delete temp;
        }
        else
        {
            Node* temp = root->right;

            while (temp->left != nullptr)
                temp = temp->left;

            root->key = temp->key;
            deleteNode(root->right, temp->key);
        }
    }
}
void LNR(Node* root)
{
    if (root != nullptr)
    {
        LNR(root->left);
        cout << root->key << " ";
        LNR(root->right);
    }
}
int main()
{
    Node* root = nullptr;

    addNode(root, 50);
    addNode(root, 30);
    addNode(root, 70);
    addNode(root, 20);
    addNode(root, 40);
    addNode(root, 60);
    addNode(root, 80);

    cout << "Cay BST: ";
    LNR(root);

    if (searchNode(root, 40) != nullptr)
        cout << "\nTim thay 40";
    else
        cout << "\nKhong tim thay 40";

    deleteNode(root, 30);

    cout << "\nSau khi xoa 30: ";
    LNR(root);

    return 0;
}