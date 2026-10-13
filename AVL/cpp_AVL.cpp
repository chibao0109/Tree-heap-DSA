#include <iostream>
#include <algorithm>

class Node {
public:
    int value;
    Node* left;
    Node* right;
    int height;

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
public:
    Node* root;

    AVLTree() : root(nullptr) {}

    // Time Complexity:  O(1)
    // Space Complexity: O(1)
    int height(Node* node) {
        if (!node) return 0;
        return node->height;
    }

    // Time Complexity:  O(1)
    // Space Complexity: O(1)
    int balance(Node* node) {
        if (!node) return 0;
        return height(node->left) - height(node->right);
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    Node* insert(Node* root, int value) {
        if (!root) return new Node(value);
        if (value < root->value) {
            root->left = insert(root->left, value);
        } else {
            root->right = insert(root->right, value);
        }

        root->height = 1 + std::max(height(root->left), height(root->right));
        int bal = balance(root);

        // Right rotation
        if (bal > 1 && value < root->left->value) {
            return right_rotate(root);
        }

        // Left rotation
        if (bal < -1 && value > root->right->value) {
            return left_rotate(root);
        }

        // Left-Right rotation
        if (bal > 1 && value > root->left->value) {
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }

        // Right-Left rotation
        if (bal < -1 && value < root->right->value) {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }

        return root;
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    Node* deleteNode(Node* root, int value) {
        if (!root) return root;

        if (value < root->value) {
            root->left = deleteNode(root->left, value);
        } else if (value > root->value) {
            root->right = deleteNode(root->right, value);
        } else {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = min_value_node(root->right); // find in-order successor
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }

        if (!root) return root;

        root->height = 1 + std::max(height(root->left), height(root->right));
        int bal = balance(root);

        // Right Rotation
        if (bal > 1 && balance(root->left) >= 0) {
            return right_rotate(root);
        }

        // Left Rotation
        if (bal < -1 && balance(root->right) <= 0) {
            return left_rotate(root);
        }

        // Left-Right rotation
        if (bal > 1 && balance(root->left) < 0) {
            root->left = left_rotate(root->left);
            return right_rotate(root);
        }

        // Right-Left rotation
        if (bal < -1 && balance(root->right) > 0) {
            root->right = right_rotate(root->right);
            return left_rotate(root);
        }

        return root;
    }

    // Time Complexity:  O(1)
    // Space Complexity: O(1)
    Node* left_rotate(Node* z) {
        Node* y = z->right;
        Node* T2 = y->left;

        y->left = z;
        z->right = T2;

        z->height = 1 + std::max(height(z->left), height(z->right));
        y->height = 1 + std::max(height(y->left), height(y->right));

        return y;
    }

    // Time Complexity:  O(1)
    // Space Complexity: O(1)
    Node* right_rotate(Node* z) {
        Node* y = z->left;
        Node* T3 = y->right;

        y->right = z;
        z->left = T3;

        z->height = 1 + std::max(height(z->left), height(z->right));
        y->height = 1 + std::max(height(y->left), height(y->right));

        return y;
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(1)
    Node* min_value_node(Node* root) {
        Node* current = root;
        while (current->left) {
            current = current->left;
        }
        return current;
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    Node* search(Node* root, int value) {
        if (!root || root->value == value) return root;
        if (root->value < value) return search(root->right, value);
        return search(root->left, value);
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    void insert_value(int value) {
        root = insert(root, value);
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    void delete_value(int value) {
        root = deleteNode(root, value);
    }

    // Time Complexity:  O(log n)
    // Space Complexity: O(log n)
    Node* search_value(int value) {
        return search(root, value);
    }
};

// Time Complexity:  O(n)
// Space Complexity: O(log n)
void inorder_traversal(Node* root) {
    if (root) {
        inorder_traversal(root->left);
        std::cout << root->value << " ";
        inorder_traversal(root->right);
    }
}

int main() {
    AVLTree tree;
    tree.insert_value(10);
    tree.insert_value(20);
    tree.insert_value(30);
    tree.insert_value(40);
    tree.insert_value(50);

    std::cout << "Tree after insertion:" << std::endl;
    inorder_traversal(tree.root);
    std::cout << std::endl;

    tree.delete_value(20);
    std::cout << "Tree after deletion of 20:" << std::endl;
    inorder_traversal(tree.root);
    std::cout << std::endl;

    Node* result = tree.search_value(30);
    if (result) {
        std::cout << "Node found" << std::endl;
    } else {
        std::cout << "Node not found" << std::endl;
    }

    return 0;
}