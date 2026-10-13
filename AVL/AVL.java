import java.util.*;

class Node {
    int value;
    Node left;
    Node right;
    int height;

    Node(int val) {
        value = val;
        left = null;
        right = null;
        height = 1;
    }
}

class AVLTree {
    Node root;

    AVLTree() {
        root = null;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    int height(Node node) {
        if (node == null) return 0;
        return node.height;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    int balance(Node node) {
        if (node == null) return 0;
        return height(node.left) - height(node.right);
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    Node insert(Node root, int value) {
        if (root == null) return new Node(value);

        if (value < root.value) {
            root.left = insert(root.left, value);
        } else {
            root.right = insert(root.right, value);
        }

        root.height = 1 + Math.max(height(root.left), height(root.right));
        int bal = balance(root);

        // Right rotation
        if (bal > 1 && value < root.left.value) {
            return rightRotate(root);
        }

        // Left rotation
        if (bal < -1 && value > root.right.value) {
            return leftRotate(root);
        }

        // Left-Right rotation
        if (bal > 1 && value > root.left.value) {
            root.left = leftRotate(root.left);
            return rightRotate(root);
        }

        // Right-Left rotation
        if (bal < -1 && value < root.right.value) {
            root.right = rightRotate(root.right);
            return leftRotate(root);
        }

        return root;
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    Node deleteNode(Node root, int value) {
        if (root == null) return root;

        if (value < root.value) {
            root.left = deleteNode(root.left, value);
        } else if (value > root.value) {
            root.right = deleteNode(root.right, value);
        } else {
            if (root.left == null) {
                Node temp = root.right;
                return temp;
            } else if (root.right == null) {
                Node temp = root.left;
                return temp;
            }

            Node temp = minValueNode(root.right);
            root.value = temp.value;
            root.right = deleteNode(root.right, temp.value);
        }

        if (root == null) return root;

        root.height = 1 + Math.max(height(root.left), height(root.right));
        int bal = balance(root);

        // Right rotation
        if (bal > 1 && balance(root.left) >= 0) {
            return rightRotate(root);
        }

        // Left rotation
        if (bal < -1 && balance(root.right) <= 0) {
            return leftRotate(root);
        }

        // Left-Right rotation
        if (bal > 1 && balance(root.left) < 0) {
            root.left = leftRotate(root.left);
            return rightRotate(root);
        }

        // Right-Left rotation
        if (bal < -1 && balance(root.right) > 0) {
            root.right = rightRotate(root.right);
            return leftRotate(root);
        }

        return root;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    Node leftRotate(Node z) {
        Node y = z.right;
        Node T2 = y.left;

        y.left = z;
        z.right = T2;

        z.height = 1 + Math.max(height(z.left), height(z.right));
        y.height = 1 + Math.max(height(y.left), height(y.right));

        return y;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    Node rightRotate(Node z) {
        Node y = z.left;
        Node T3 = y.right;

        y.right = z;
        z.left = T3;

        z.height = 1 + Math.max(height(z.left), height(z.right));
        y.height = 1 + Math.max(height(y.left), height(y.right));

        return y;
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(1)
    Node minValueNode(Node root) {
        Node current = root;

        while (current.left != null) {
            current = current.left;
        }

        return current;
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    Node search(Node root, int value) {
        if (root == null || root.value == value) return root;

        if (root.value < value) {
            return search(root.right, value);
        }

        return search(root.left, value);
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    void insertValue(int value) {
        root = insert(root, value);
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    void deleteValue(int value) {
        root = deleteNode(root, value);
    }

    // Time Complexity: O(log n)
    // Space Complexity: O(log n)
    Node searchValue(int value) {
        return search(root, value);
    }
}

// Time Complexity: O(n)
// Space Complexity: O(log n)
public class Main {
    static void inorderTraversal(Node root) {
        if (root != null) {
            inorderTraversal(root.left);
            System.out.print(root.value + " ");
            inorderTraversal(root.right);
        }
    }

    public static void main(String[] args) {
        AVLTree tree = new AVLTree();

        tree.insertValue(10);
        tree.insertValue(20);
        tree.insertValue(30);
        tree.insertValue(40);
        tree.insertValue(50);

        System.out.println("Tree after insertion:");
        inorderTraversal(tree.root);
        System.out.println();

        tree.deleteValue(20);

        System.out.println("Tree after deletion of 20:");
        inorderTraversal(tree.root);
        System.out.println();

        Node result = tree.searchValue(30);

        if (result != null) {
            System.out.println("Node found");
        } else {
            System.out.println("Node not found");
        }
    }
}
