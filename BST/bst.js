// =======================
// 1. Tạo Node
// =======================
class Node {
    constructor(x) {
        this.val = x;
        this.left = null;
        this.right = null;
    }
}

// =======================
// 2. INSERT
// =======================
function insert(root, x) {
    // Cây con đang rỗng
    if (root === null) {
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
function search(root, x) {
    if (root === null) {
        return false;
    }

    if (x === root.val) {
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
function findMin(root) {
    while (root.left !== null) {
        root = root.left;
    }
    return root;
}

// =======================
// 5. FIND MAX
// =======================
function findMax(root) {
    while (root.right !== null) {
        root = root.right;
    }
    return root;
}

// =======================
// 6. DELETE
// =======================
function deleteNode(root, x) {
    // Không tìm thấy
    if (root === null) {
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
        if (root.left === null && root.right === null) {
            return null;
        }
        // Case 2: Chỉ có con phải
        else if (root.left === null) {
            return root.right;
        }
        // Case 3: Chỉ có con trái
        else if (root.right === null) {
            return root.left;
        }
        // Case 4: Có cả hai con
        else {
            const temp = findMin(root.right);
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
function inorder(root, result) {
    if (root === null) {
        return;
    }
    inorder(root.left, result);
    result.push(root.val);
    inorder(root.right, result);
}

// =======================
// 8. PREORDER
// Root -> Left -> Right
// =======================
function preorder(root, result) {
    if (root === null) {
        return;
    }
    result.push(root.val);
    preorder(root.left, result);
    preorder(root.right, result);
}

// =======================
// 9. POSTORDER
// Left -> Right -> Root
// =======================
function postorder(root, result) {
    if (root === null) {
        return;
    }
    postorder(root.left, result);
    postorder(root.right, result);
    result.push(root.val);
}

// =======================
// MAIN
// =======================
function main() {
    let root = null;

    // Tạo BST
    const values = [8, 3, 10, 1, 6, 14, 4, 7];
    for (const x of values) {
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
    let result = [];
    inorder(root, result);
    console.log("Inorder: " + result.join(" "));

    result = [];
    preorder(root, result);
    console.log("Preorder: " + result.join(" "));

    result = [];
    postorder(root, result);
    console.log("Postorder: " + result.join(" "));

    // Search
    const x = 6;
    if (search(root, x)) {
        console.log(x + " ton tai trong cay");
    } else {
        console.log(x + " khong ton tai trong cay");
    }

    // Min / Max
    console.log("Min: " + findMin(root).val);
    console.log("Max: " + findMax(root).val);

    // Delete
    root = deleteNode(root, 3);

    result = [];
    inorder(root, result);
    console.log("Inorder sau khi xoa 3: " + result.join(" "));
}

main();
