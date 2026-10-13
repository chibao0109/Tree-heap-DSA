#include <bits/stdc++.h>
using namespace std;

// =======================
// 1. Tạo Node
// =======================
struct Node {
    int val;
    Node* left;
    Node* right;

    Node(int x) {
        val = x;
        left = NULL;
        right = NULL;
    }
};

// =======================
// 2. INSERT
// =======================
Node* insert(Node* root, int x) {

    // Cây con đang rỗng
    if (root == NULL) {
        return new Node(x);
    }

    // x nhỏ hơn root → đi sang trái
    if (x < root->val) {
        root->left = insert(root->left, x);
    }

    // x lớn hơn root → đi sang phải
    else if (x > root->val) {
        root->right = insert(root->right, x);
    }

    // x == root->val → không thêm
    return root;
}

// =======================
// 3. SEARCH
// =======================
bool search(Node* root, int x) {

    if (root == NULL) {
        return false;
    }

    if (x == root->val) {
        return true;
    }

    if (x < root->val) {
        return search(root->left, x);
    }

    return search(root->right, x);
}

// =======================
// 4. FIND MIN
// =======================
Node* findMin(Node* root) {

    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}

// =======================
// 5. FIND MAX
// =======================
Node* findMax(Node* root) {

    while (root->right != NULL) {
        root = root->right;
    }

    return root;
}

// =======================
// 6. DELETE
// =======================
Node* deleteNode(Node* root, int x) {

    // Không tìm thấy
    if (root == NULL) {
        return NULL;
    }

    // x nhỏ hơn root → đi trái
    if (x < root->val) {
        root->left = deleteNode(root->left, x);
    }

    // x lớn hơn root → đi phải
    else if (x > root->val) {
        root->right = deleteNode(root->right, x);
    }

    // Đã tìm thấy node cần xóa
    else {

        // Case 1: Không có con
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }

        // Case 2: Chỉ có con phải
        else if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        // Case 3: Chỉ có con trái
        else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // Case 4: Có cả hai con
        else {
            Node* temp = findMin(root->right);

            root->val = temp->val;

            root->right = deleteNode(root->right, temp->val);
        }
    }

    return root;
}

// =======================
// 7. INORDER
// Left → Root → Right
// =======================
void inorder(Node* root) {

    if (root == NULL) {
        return;
    }

    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

// =======================
// 8. PREORDER
// Root → Left → Right
// =======================
void preorder(Node* root) {

    if (root == NULL) {
        return;
    }

    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
}

// =======================
// 9. POSTORDER
// Left → Right → Root
// =======================
void postorder(Node* root) {

    if (root == NULL) {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
}

// =======================
// MAIN
// =======================
int main() {

    Node* root = NULL;

    // Tạo BST
    root = insert(root, 8);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 1);
    root = insert(root, 6);
    root = insert(root, 14);
    root = insert(root, 4);
    root = insert(root, 7);

    /*
            8
           / \
          3   10
         / \    \
        1   6    14
           / \
          4   7
    */

    // Duyệt cây
    cout << "Inorder: ";
    inorder(root);
    cout << endl;

    cout << "Preorder: ";
    preorder(root);
    cout << endl;

    cout << "Postorder: ";
    postorder(root);
    cout << endl;

    // Search
    int x = 6;

    if (search(root, x)) {
        cout << x << " ton tai trong cay\n";
    }
    else {
        cout << x << " khong ton tai trong cay\n";
    }

    // Min / Max
    cout << "Min: " << findMin(root)->val << endl;
    cout << "Max: " << findMax(root)->val << endl;

    // Delete
    root = deleteNode(root, 3);

    cout << "Inorder sau khi xoa 3: ";
    inorder(root);
    cout << endl;

    return 0;
}