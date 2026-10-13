import java.util.LinkedList;
import java.util.Queue;

class Node {
    int data;
    Node left;
    Node right;

    Node(int value) {
        data = value;
        left = null;
        right = null;
    }
}

public class Main {

    // Preorder: Root, Left, Right
    static void preorder(Node root) {
        if (root == null) return;

        System.out.print(root.data + " ");
        preorder(root.left);
        preorder(root.right);
    }

    // Inorder: Left, Root, Right
    static void inorder(Node root) {
        if (root == null) return;

        inorder(root.left);
        System.out.print(root.data + " ");
        inorder(root.right);
    }

    // Postorder: Left, Right, Root
    static void postorder(Node root) {
        if (root == null) return;

        postorder(root.left);
        postorder(root.right);
        System.out.print(root.data + " ");
    }

    // Level-order: duyet tung tang bang Queue
    static void levelOrder(Node root) {
        if (root == null) return;

        Queue<Node> q = new LinkedList<>();
        q.add(root);

        while (!q.isEmpty()) {
            Node current = q.remove();

            System.out.print(current.data + " ");

            if (current.left != null)
                q.add(current.left);

            if (current.right != null)
                q.add(current.right);
        }
    }

    // Dem so node
    static int countNodes(Node root) {
        if (root == null) return 0;

        return 1 + countNodes(root.left)
                 + countNodes(root.right);
    }

    public static void main(String[] args) {
        // Tao cay
        Node root = new Node(1);

        root.left = new Node(2);
        root.right = new Node(3);

        root.left.left = new Node(4);
        root.left.right = new Node(5);

        root.right.left = new Node(6);
        root.right.right = new Node(7);

        System.out.print("Preorder: ");
        preorder(root);

        System.out.print("\nInorder: ");
        inorder(root);

        System.out.print("\nPostorder: ");
        postorder(root);

        System.out.print("\nLevel-order: ");
        levelOrder(root);

        System.out.print("\nNumber of nodes: ");
        System.out.print(countNodes(root));
    }
}


