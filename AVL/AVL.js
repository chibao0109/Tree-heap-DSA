class Node {
  constructor(value) {
    this.value = value;
    this.left = null;
    this.right = null;
    this.height = 1;
  }
}

class AVLTree {
  constructor() {
    this.root = null;
  }

  // Time Complexity:  O(1)
  // Space Complexity: O(1)
  height(node) {
    if (!node) return 0;
    return node.height;
  }

  // Time Complexity:  O(1)
  // Space Complexity: O(1)
  balance(node) {
    if (!node) return 0;
    return this.height(node.left) - this.height(node.right);
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  insert(root, value) {
    if (!root) return new Node(value);

    if (value < root.value) {
      root.left = this.insert(root.left, value);
    } else {
      root.right = this.insert(root.right, value);
    }

    root.height = 1 + Math.max(this.height(root.left), this.height(root.right));
    const bal = this.balance(root);

    // Right rotation
    if (bal > 1 && value < root.left.value) {
      return this.rightRotate(root);
    }

    // Left rotation
    if (bal < -1 && value > root.right.value) {
      return this.leftRotate(root);
    }

    // Left-Right rotation
    if (bal > 1 && value > root.left.value) {
      root.left = this.leftRotate(root.left);
      return this.rightRotate(root);
    }

    // Right-Left rotation
    if (bal < -1 && value < root.right.value) {
      root.right = this.rightRotate(root.right);
      return this.leftRotate(root);
    }

    return root;
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  delete(root, value) {
    if (!root) return root;

    if (value < root.value) {
      root.left = this.delete(root.left, value);
    } else if (value > root.value) {
      root.right = this.delete(root.right, value);
    } else {
      if (!root.left) {
        return root.right;
      } else if (!root.right) {
        return root.left;
      }

      const temp = this.minValueNode(root.right); // find in-order successor
      root.value = temp.value;
      root.right = this.delete(root.right, temp.value);
    }

    if (!root) return root;

    root.height = 1 + Math.max(this.height(root.left), this.height(root.right));
    const bal = this.balance(root);

    // Right Rotation
    if (bal > 1 && this.balance(root.left) >= 0) {
      return this.rightRotate(root);
    }

    // Left Rotation
    if (bal < -1 && this.balance(root.right) <= 0) {
      return this.leftRotate(root);
    }

    // Left-Right rotation
    if (bal > 1 && this.balance(root.left) < 0) {
      root.left = this.leftRotate(root.left);
      return this.rightRotate(root);
    }

    // Right-Left rotation
    if (bal < -1 && this.balance(root.right) > 0) {
      root.right = this.rightRotate(root.right);
      return this.leftRotate(root);
    }

    return root;
  }

  // Time Complexity:  O(1)
  // Space Complexity: O(1)
  leftRotate(z) {
    const y = z.right;
    const T2 = y.left;

    y.left = z;
    z.right = T2;

    z.height = 1 + Math.max(this.height(z.left), this.height(z.right));
    y.height = 1 + Math.max(this.height(y.left), this.height(y.right));

    return y;
  }

  // Time Complexity:  O(1)
  // Space Complexity: O(1)
  rightRotate(z) {
    const y = z.left;
    const T3 = y.right;

    y.right = z;
    z.left = T3;

    z.height = 1 + Math.max(this.height(z.left), this.height(z.right));
    y.height = 1 + Math.max(this.height(y.left), this.height(y.right));

    return y;
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(1)
  minValueNode(root) {
    let current = root;
    while (current.left) {
      current = current.left;
    }
    return current;
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  search(root, value) {
    if (!root || root.value === value) return root;
    if (root.value < value) return this.search(root.right, value);
    return this.search(root.left, value);
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  insertValue(value) {
    this.root = this.insert(this.root, value);
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  deleteValue(value) {
    this.root = this.delete(this.root, value);
  }

  // Time Complexity:  O(log n)
  // Space Complexity: O(log n)
  searchValue(value) {
    return this.search(this.root, value);
  }
}

// Time Complexity:  O(n)
// Space Complexity: O(log n)
function inorderTraversal(root, output = []) {
  if (root) {
    inorderTraversal(root.left, output);
    output.push(root.value);
    inorderTraversal(root.right, output);
  }
  return output;
}

// Example usage:
const tree = new AVLTree();
tree.insertValue(10);
tree.insertValue(20);
tree.insertValue(30);
tree.insertValue(40);
tree.insertValue(50);

console.log("Tree after insertion:");
console.log(inorderTraversal(tree.root).join(" "));

tree.deleteValue(20);
console.log("Tree after deletion of 20:");
console.log(inorderTraversal(tree.root).join(" "));

const result = tree.searchValue(30);
if (result) {
  console.log("Node found");
} else {
  console.log("Node not found");
}
