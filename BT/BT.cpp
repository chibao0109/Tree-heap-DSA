
#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// Preorder: Root -> Left -> Right
void preorder(Node* root) {
    if (root == nullptr) return;

    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

// Inorder: Left -> Root -> Right
void inorder(Node* root) {
    if (root == nullptr) return;

    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

// Postorder: Left -> Right -> Root
void postorder(Node* root) {
    if (root == nullptr) return;

    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

// Level-order: duyet tung tang bang Queue
void levelOrder(Node* root) {
    if (root == nullptr) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->data << " ";

        if (current->left != nullptr)
            q.push(current->left);

        if (current->right != nullptr)
            q.push(current->right);
    }
}

// Tim kiem trong Binary Tree thuong
bool search(Node* root, int value) {
    if (root == nullptr) return false;

    if (root->data == value) return true;

    return search(root->left, value) ||
           search(root->right, value);
}

// Dem so node
int countNodes(Node* root) {
    if (root == nullptr) return 0;

    return 1 + countNodes(root->left)
             + countNodes(root->right);
}

int main() {
    // Tao cay
    Node* root = new Node(10);

    root->left = new Node(5);
    root->right = new Node(20);

    root->left->left = new Node(3);
    root->left->right = new Node(7);

    root->right->left = new Node(15);
    root->right->right = new Node(30);

    cout << "Preorder: ";
    preorder(root);

    cout << "\nInorder: ";
    inorder(root);

    cout << "\nPostorder: ";
    postorder(root);

    cout << "\nLevel-order: ";
    levelOrder(root);

    cout << "\nNumber of nodes: "
         << countNodes(root);

    int value = 15;

    if (search(root, value))
        cout << "\nFound " << value;
    else
        cout << "\nNot found";

    return 0;
}