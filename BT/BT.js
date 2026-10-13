class Node {
    constructor(value) {
        this.data = value;
        this.left = null;
        this.right = null;
    }
}

// Preorder: Root, Left, Right
function preorder(root) {
    if (root === null) return;

    process.stdout.write(root.data + " ");
    preorder(root.left);
    preorder(root.right);
}

// Inorder: Left, Root, Right
function inorder(root) {
    if (root === null) return;

    inorder(root.left);
    process.stdout.write(root.data + " ");
    inorder(root.right);
}

// Postorder: Left, Right, Root
function postorder(root) {
    if (root === null) return;

    postorder(root.left);
    postorder(root.right);
    process.stdout.write(root.data + " ");
}

// Level-order: duyet tung tang bang Queue
function levelOrder(root) {
    if (root === null) return;

    let q = [root];
    let index = 0;

    while (index < q.length) {
        let current = q[index++];

        process.stdout.write(current.data + " ");

        if (current.left !== null)
            q.push(current.left);

        if (current.right !== null)
            q.push(current.right);
    }
}

// Dem so node
function countNodes(root) {
    if (root === null) return 0;

    return 1 + countNodes(root.left)
             + countNodes(root.right);
}

// Tao cay
const root = new Node(1);

root.left = new Node(2);
root.right = new Node(3);

root.left.left = new Node(4);
root.left.right = new Node(5);

root.right.left = new Node(6);
root.right.right = new Node(7);

process.stdout.write("Preorder: ");
preorder(root);

process.stdout.write("\nInorder: ");
inorder(root);

process.stdout.write("\nPostorder: ");
postorder(root);

process.stdout.write("\nLevel-order: ");
levelOrder(root);

process.stdout.write("\nNumber of nodes: ");
process.stdout.write(countNodes(root).toString());