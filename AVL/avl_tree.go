package main

import (
	"fmt"
	"math"
)

type Node struct {
	value  int
	left   *Node
	right  *Node
	height int
}

func NewNode(val int) *Node {
	return &Node{
		value:  val,
		left:   nil,
		right:  nil,
		height: 1,
	}
}

type AVLTree struct {
	root *Node
}

// Time Complexity: O(1)
// Space Complexity: O(1)
func (tree *AVLTree) height(node *Node) int {
	if node == nil {
		return 0
	}
	return node.height
}

// Time Complexity: O(1)
// Space Complexity: O(1)
func (tree *AVLTree) balance(node *Node) int {
	if node == nil {
		return 0
	}
	return tree.height(node.left) - tree.height(node.right)
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) insert(root *Node, value int) *Node {
	if root == nil {
		return NewNode(value)
	}

	if value < root.value {
		root.left = tree.insert(root.left, value)
	} else {
		root.right = tree.insert(root.right, value)
	}

	root.height = 1 + int(math.Max(float64(tree.height(root.left)), float64(tree.height(root.right))))
	bal := tree.balance(root)

	// Right rotation
	if bal > 1 && value < root.left.value {
		return tree.rightRotate(root)
	}

	// Left rotation
	if bal < -1 && value > root.right.value {
		return tree.leftRotate(root)
	}

	// Left-Right rotation
	if bal > 1 && value > root.left.value {
		root.left = tree.leftRotate(root.left)
		return tree.rightRotate(root)
	}

	// Right-Left rotation
	if bal < -1 && value < root.right.value {
		root.right = tree.rightRotate(root.right)
		return tree.leftRotate(root)
	}

	return root
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) deleteNode(root *Node, value int) *Node {
	if root == nil {
		return root
	}

	if value < root.value {
		root.left = tree.deleteNode(root.left, value)
	} else if value > root.value {
		root.right = tree.deleteNode(root.right, value)
	} else {
		if root.left == nil {
			temp := root.right
			return temp
		} else if root.right == nil {
			temp := root.left
			return temp
		}

		temp := tree.minValueNode(root.right)
		root.value = temp.value
		root.right = tree.deleteNode(root.right, temp.value)
	}

	if root == nil {
		return root
	}

	root.height = 1 + int(math.Max(float64(tree.height(root.left)), float64(tree.height(root.right))))
	bal := tree.balance(root)

	// Right rotation
	if bal > 1 && tree.balance(root.left) >= 0 {
		return tree.rightRotate(root)
	}

	// Left rotation
	if bal < -1 && tree.balance(root.right) <= 0 {
		return tree.leftRotate(root)
	}

	// Left-Right rotation
	if bal > 1 && tree.balance(root.left) < 0 {
		root.left = tree.leftRotate(root.left)
		return tree.rightRotate(root)
	}

	// Right-Left rotation
	if bal < -1 && tree.balance(root.right) > 0 {
		root.right = tree.rightRotate(root.right)
		return tree.leftRotate(root)
	}

	return root
}

// Time Complexity: O(1)
// Space Complexity: O(1)
func (tree *AVLTree) leftRotate(z *Node) *Node {
	y := z.right
	T2 := y.left

	y.left = z
	z.right = T2

	z.height = 1 + int(math.Max(float64(tree.height(z.left)), float64(tree.height(z.right))))
	y.height = 1 + int(math.Max(float64(tree.height(y.left)), float64(tree.height(y.right))))

	return y
}

// Time Complexity: O(1)
// Space Complexity: O(1)
func (tree *AVLTree) rightRotate(z *Node) *Node {
	y := z.left
	T3 := y.right

	y.right = z
	z.left = T3

	z.height = 1 + int(math.Max(float64(tree.height(z.left)), float64(tree.height(z.right))))
	y.height = 1 + int(math.Max(float64(tree.height(y.left)), float64(tree.height(y.right))))

	return y
}

// Time Complexity: O(log n)
// Space Complexity: O(1)
func (tree *AVLTree) minValueNode(root *Node) *Node {
	current := root
	for current.left != nil {
		current = current.left
	}
	return current
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) search(root *Node, value int) *Node {
	if root == nil || root.value == value {
		return root
	}

	if root.value < value {
		return tree.search(root.right, value)
	}

	return tree.search(root.left, value)
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) insertValue(value int) {
	tree.root = tree.insert(tree.root, value)
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) deleteValue(value int) {
	tree.root = tree.deleteNode(tree.root, value)
}

// Time Complexity: O(log n)
// Space Complexity: O(log n)
func (tree *AVLTree) searchValue(value int) *Node {
	return tree.search(tree.root, value)
}

// Time Complexity: O(n)
// Space Complexity: O(log n)
func inorderTraversal(root *Node) {
	if root != nil {
		inorderTraversal(root.left)
		fmt.Printf("%d ", root.value)
		inorderTraversal(root.right)
	}
}

func main() {
	tree := AVLTree{}

	tree.insertValue(10)
	tree.insertValue(20)
	tree.insertValue(30)
	tree.insertValue(40)
	tree.insertValue(50)

	fmt.Println("Tree after insertion:")
	inorderTraversal(tree.root)
	fmt.Println()

	tree.deleteValue(20)

	fmt.Println("Tree after deletion of 20:")
	inorderTraversal(tree.root)
	fmt.Println()

	result := tree.searchValue(30)
	if result != nil {
		fmt.Println("Node found")
	} else {
		fmt.Println("Node not found")
	}
}
