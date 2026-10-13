import java.util.ArrayList;
import java.util.List;

public class BST {

    // =======================
    // 1. Tạo Node
    // =======================
    static class Node {
        int val;
        Node left;
        Node right;

        Node(int x) {
            val = x;
            left = null;
            right = null;
        }
    }

    // =======================
    // 2. INSERT
    // =======================
    static Node insert(Node root, int x) {
        // Cây con đang rỗng
        if (root == null) {
            return new Node(x);
        }

        // x nhỏ hơn root -> đi sang trái
        if (x < root.val) {
            root.left = insert(root.left, x);
        }
        // x lớn hơn root -> đi sang phải
        else if (x > root.val) {
            root.right = insert(root.right, x);
        }
        // x == root.val -> không thêm

        return root;
    }

    // =======================
    // 3. SEARCH
    // =======================
    static boolean search(Node root, int x) {
        if (root == null) {
            return false;
        }

        if (x == root.val) {
            return true;
        }

        if (x < root.val) {
            return search(root.left, x);
        }

        return search(root.right, x);
    }

    // =======================
    // 4. FIND MIN
    // =======================
    static Node findMin(Node root) {
        while (root.left != null) {
            root = root.left;
        }
        return root;
    }

    // =======================
    // 5. FIND MAX
    // =======================
    static Node findMax(Node root) {
        while (root.right != null) {
            root = root.right;
        }
        return root;
    }

    // =======================
    // 6. DELETE
    // =======================
    static Node deleteNode(Node root, int x) {
        // Không tìm thấy
        if (root == null) {
            return null;
        }

        // x nhỏ hơn root -> đi trái
        if (x < root.val) {
            root.left = deleteNode(root.left, x);
        }
        // x lớn hơn root -> đi phải
        else if (x > root.val) {
            root.right = deleteNode(root.right, x);
        }
        // Đã tìm thấy node cần xóa
        else {
            // Case 1: Không có con
            if (root.left == null && root.right == null) {
                return null;
            }
            // Case 2: Chỉ có con phải
            else if (root.left == null) {
                return root.right;
            }
            // Case 3: Chỉ có con trái
            else if (root.right == null) {
                return root.left;
            }
            // Case 4: Có cả hai con
            else {
                Node temp = findMin(root.right);
                root.val = temp.val;
                root.right = deleteNode(root.right, temp.val);
            }
        }

        return root;
    }

    // =======================
    // 7. INORDER
    // Left -> Root -> Right
    // =======================
    static void inorder(Node root, List<Integer> result) {
        if (root == null) {
            return;
        }
        inorder(root.left, result);
        result.add(root.val);
        inorder(root.right, result);
    }

    // =======================
    // 8. PREORDER
    // Root -> Left -> Right
    // =======================
    static void preorder(Node root, List<Integer> result) {
        if (root == null) {
            return;
        }
        result.add(root.val);
        preorder(root.left, result);
        preorder(root.right, result);
    }

    // =======================
    // 9. POSTORDER
    // Left -> Right -> Root
    // =======================
    static void postorder(Node root, List<Integer> result) {
        if (root == null) {
            return;
        }
        postorder(root.left, result);
        postorder(root.right, result);
        result.add(root.val);
    }

    // Helper: in list dạng "a b c"
    static String join(List<Integer> list) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            if (i > 0) sb.append(" ");
            sb.append(list.get(i));
        }
        return sb.toString();
    }

    // =======================
    // MAIN
    // =======================
    public static void main(String[] args) {
        Node root = null;

        // Tạo BST
        int[] values = {8, 3, 10, 1, 6, 14, 4, 7};
        for (int x : values) {
            root = insert(root, x);
        }

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
        List<Integer> result = new ArrayList<>();
        inorder(root, result);
        System.out.println("Inorder: " + join(result));

        result = new ArrayList<>();
        preorder(root, result);
        System.out.println("Preorder: " + join(result));

        result = new ArrayList<>();
        postorder(root, result);
        System.out.println("Postorder: " + join(result));

        // Search
        int x = 6;
        if (search(root, x)) {
            System.out.println(x + " ton tai trong cay");
        } else {
            System.out.println(x + " khong ton tai trong cay");
        }

        // Min / Max
        System.out.println("Min: " + findMin(root).val);
        System.out.println("Max: " + findMax(root).val);

        // Delete
        root = deleteNode(root, 3);

        result = new ArrayList<>();
        inorder(root, result);
        System.out.println("Inorder sau khi xoa 3: " + join(result));
    }
}
