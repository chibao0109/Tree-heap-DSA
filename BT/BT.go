package main

import "fmt"

type Node struct {
    data  int
    left  *Node
    right *Node
}

// Preorder: Root, Left, Right
func preorder(root *Node) {
    if root == nil {
        return
    }

    fmt.Print(root.data, " ")
    preorder(root.left)
    preorder(root.right)
}

// Inorder: Left, Root, Right
func inorder(root *Node) {
    if root == nil {
        return
    }

    inorder(root.left)
    fmt.Print(root.data, " ")
    inorder(root.right)
}

// Postorder: Left, Right, Root
func postorder(root *Node) {
    if root == nil {
        return
    }

    postorder(root.left)
    postorder(root.right)
    fmt.Print(root.data, " ")
}

// Level-order: duyet tung tang bang Queue
func levelOrder(root *Node) {
    if root == nil {
        return
    }

    q := []*Node{root}

    for len(q) > 0 {
        current := q[0]
        q = q[1:]

        fmt.Print(current.data, " ")

        if current.left != nil {
            q = append(q, current.left)
        }

        if current.right != nil {
            q = append(q, current.right)
        }
    }
}

// Dem so node
func countNodes(root *Node) int {
    if root == nil {
        return 0
    }

    return 1 + countNodes(root.left) +
        countNodes(root.right)
}

func main() {
    // Tao cay
    root := &Node{data: 1}

    root.left = &Node{data: 2}
    root.right = &Node{data: 3}

    root.left.left = &Node{data: 4}
    root.left.right = &Node{data: 5}

    root.right.left = &Node{data: 6}
    root.right.right = &Node{data: 7}

    fmt.Print("Preorder: ")
    preorder(root)

    fmt.Print("\nInorder: ")
    inorder(root)

    fmt.Print("\nPostorder: ")
    postorder(root)

    fmt.Print("\nLevel-order: ")
    levelOrder(root)

    fmt.Print("\nNumber of nodes: ")
    fmt.Print(countNodes(root))
}