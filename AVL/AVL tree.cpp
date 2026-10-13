#include <iostream>
using namespace std;

// ================= NODE =================
struct Node {
    int key;
    Node* left;
    Node* right;
    int height; // chiều cao của node (dùng để tính balance factor)

    Node(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

// ================= HÀM PHỤ TRỢ =================

// Lấy chiều cao của node (node null coi như height = 0)
int getHeight(Node* n) {
    if (n == nullptr) return 0;
    return n->height;
}

// Tính balance factor = height(left) - height(right)
int getBalance(Node* n) {
    if (n == nullptr) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Cập nhật lại height sau khi thay đổi cấu trúc cây
void updateHeight(Node* n) {
    n->height = 1 + max(getHeight(n->left), getHeight(n->right));
}

// ================= CÁC PHÉP XOAY =================

// Xoay phải (Right Rotation) - dùng khi bị lệch trái (LL case)
//        y                          x
//       / \                       /   \
//      x   T3     ---->         T1     y
//     / \                             / \
//    T1  T2                         T2  T3
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Thực hiện xoay
    x->right = y;
    y->left = T2;

    // Cập nhật height (phải cập nhật y trước vì y giờ ở dưới)
    updateHeight(y);
    updateHeight(x);

    return x; // x trở thành root mới của subtree này
}

// Xoay trái (Left Rotation) - dùng khi bị lệch phải (RR case)
//    x                              y
//   / \                           /   \
//  T1  y         ---->           x     T3
//     / \                       / \
//    T2 T3                    T1  T2
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Thực hiện xoay
    y->left = x;
    x->right = T2;

    // Cập nhật height (phải cập nhật x trước vì x giờ ở dưới)
    updateHeight(x);
    updateHeight(y);

    return y; // y trở thành root mới của subtree này
}

// ================= CHÈN (INSERT) =================
Node* insert(Node* node, int key) {
    // 1. Chèn như BST bình thường
    if (node == nullptr)
        return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // không cho phép key trùng

    // 2. Cập nhật height của node hiện tại
    updateHeight(node);

    // 3. Tính balance factor để kiểm tra node có bị mất cân bằng không
    int balance = getBalance(node);

    // 4. Nếu mất cân bằng, xét 4 trường hợp

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Không mất cân bằng, trả về node như cũ
    return node;
}

// Tìm node nhỏ nhất trong subtree (dùng cho xóa)
Node* findMin(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// ================= XÓA (DELETE) =================
Node* deleteNode(Node* root, int key) {
    if (root == nullptr) return root;

    // 1. Xóa như BST bình thường
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Node cần xóa được tìm thấy
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) { // không có con
                temp = root;
                root = nullptr;
            } else { // có 1 con
                *root = *temp; // copy nội dung con lên
            }
            delete temp;
        } else {
            // Có 2 con: lấy successor (node nhỏ nhất bên phải)
            Node* temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr) return root; // cây rỗng sau khi xóa

    // 2. Cập nhật height
    updateHeight(root);

    // 3. Kiểm tra balance factor và cân bằng lại nếu cần
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// ================= DUYỆT CÂY (IN-ORDER) =================
void inOrder(Node* root) {
    if (root == nullptr) return;
    inOrder(root->left);
    cout << root->key << " ";
    inOrder(root->right);
}

// ================= IN CÂY TRỰC QUAN (dùng để demo) =================
void printTree(Node* root, string prefix = "", bool isLeft = false) {
    if (root == nullptr) return;

    printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
    cout << prefix << (isLeft ? "└── " : "┌── ") << root->key
         << " (h=" << root->height << ")" << endl;
    printTree(root->left, prefix + (isLeft ? "    " : "│   "), true);
}

// ================= MAIN - DEMO =================
int main() {
    Node* root = nullptr;

    // Chèn một dãy số để test tự cân bằng
    int values[] = {10, 20, 30, 40, 50, 25};
    for (int v : values) {
        root = insert(root, v);
        cout << "Sau khi chen " << v << ":\n";
        printTree(root);
        cout << "Balance factor cua root: " << getBalance(root) << "\n\n";
    }

    cout << "Duyet In-order (phai la day tang dan): ";
    inOrder(root);
    cout << "\n\n";

    // Demo xóa
    cout << "Xoa node 30:\n";
    root = deleteNode(root, 30);
    printTree(root);

    return 0;
}