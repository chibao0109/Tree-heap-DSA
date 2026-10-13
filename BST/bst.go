package main

import (
	"fmt"
	"strconv"
	"strings"
)

// =======================
// 1. Tạo Node
// =======================
type Node struct {
	val   int
	left  *Node
	right *Node
}

func newNode(x int) *Node {
	return &Node{val: x, left: nil, right: nil}
}

// =======================
// 2. INSERT
// =======================
func insert(root *Node, x int) *Node {
	// Cây con đang rỗng
	if root == nil {
		return newNode(x)
	}

	// x nhỏ hơn root -> đi sang trái
	if x < root.val {
		root.left = insert(root.left, x)
	} else if x > root.val {
		// x lớn hơn root -> đi sang phải
		root.right = insert(root.right, x)
	}
	// x == root.val -> không thêm

	return root
}

// =======================
// 3. SEARCH
// =======================
func search(root *Node, x int) bool {
	if root == nil {
		return false
	}

	if x == root.val {
		return true
	}

	if x < root.val {
		return search(root.left, x)
	}

	return search(root.right, x)
}

// =======================
// 4. FIND MIN
// =======================
func findMin(root *Node) *Node {
	for root.left != nil {
		root = root.left
	}
	return root
}

// =======================
// 5. FIND MAX
// =======================
func findMax(root *Node) *Node {
	for root.right != nil {
		root = root.right
	}
	return root
}

// =======================
// 6. DELETE
// =======================
func deleteNode(root *Node, x int) *Node {
	// Không tìm thấy
	if root == nil {
		return nil
	}

	// x nhỏ hơn root -> đi trái
	if x < root.val {
		root.left = deleteNode(root.left, x)
	} else if x > root.val {
		// x lớn hơn root -> đi phải
		root.right = deleteNode(root.right, x)
	} else {
		// Đã tìm thấy node cần xóa

		// Case 1: Không có con
		if root.left == nil && root.right == nil {
			return nil
		}

		// Case 2: Chỉ có con phải
		if root.left == nil {
			return root.right
		}

		// Case 3: Chỉ có con trái
		if root.right == nil {
			return root.left
		}

		// Case 4: Có cả hai con
		temp := findMin(root.right)
		root.val = temp.val
		root.right = deleteNode(root.right, temp.val)
	}

	return root
}

// =======================
// 7. INORDER
// Left -> Root -> Right
// =======================
func inorder(root *Node, result *[]int) {
	if root == nil {
		return
	}
	inorder(root.left, result)
	*result = append(*result, root.val)
	inorder(root.right, result)
}

// =======================
// 8. PREORDER
// Root -> Left -> Right
// =======================
func preorder(root *Node, result *[]int) {
	if root == nil {
		return
	}
	*result = append(*result, root.val)
	preorder(root.left, result)
	preorder(root.right, result)
}

// =======================
// 9. POSTORDER
// Left -> Right -> Root
// =======================
func postorder(root *Node, result *[]int) {
	if root == nil {
		return
	}
	postorder(root.left, result)
	postorder(root.right, result)
	*result = append(*result, root.val)
}

// Helper: nối slice int thành chuỗi "a b c"
func joinInts(nums []int) string {
	parts := make([]string, len(nums))
	for i, n := range nums {
		parts[i] = strconv.Itoa(n)
	}
	return strings.Join(parts, " ")
}

// =======================
// MAIN
// =======================
func main() {
	var root *Node = nil

	// Tạo BST
	values := []int{8, 3, 10, 1, 6, 14, 4, 7}
	for _, x := range values {
		root = insert(root, x)
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
	var result []int
	inorder(root, &result)
	fmt.Println("Inorder:", joinInts(result))

	result = nil
	preorder(root, &result)
	fmt.Println("Preorder:", joinInts(result))

	result = nil
	postorder(root, &result)
	fmt.Println("Postorder:", joinInts(result))

	// Search
	x := 6
	if search(root, x) {
		fmt.Println(x, "ton tai trong cay")
	} else {
		fmt.Println(x, "khong ton tai trong cay")
	}

	// Min / Max
	fmt.Println("Min:", findMin(root).val)
	fmt.Println("Max:", findMax(root).val)

	// Delete
	root = deleteNode(root, 3)

	result = nil
	inorder(root, &result)
	fmt.Println("Inorder sau khi xoa 3:", joinInts(result))
}
