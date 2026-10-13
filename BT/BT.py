
from collections import deque


class Node:
    def __init__(self, value):
        self.data = value
        self.left = None
        self.right = None


# Preorder: Root -> Left -> Right
def preorder(root):
    if root is None:
        return

    print(root.data, end=" ")
    preorder(root.left)
    preorder(root.right)


# Inorder: Left -> Root -> Right
def inorder(root):
    if root is None:
        return

    inorder(root.left)
    print(root.data, end=" ")
    inorder(root.right)


# Postorder: Left -> Right -> Root
def postorder(root):
    if root is None:
        return

    postorder(root.left)
    postorder(root.right)
    print(root.data, end=" ")


# Level-order: duyet tung tang
def level_order(root):
    if root is None:
        return

    q = deque([root])

    while q:
        current = q.popleft()
        print(current.data, end=" ")

        if current.left is not None:
            q.append(current.left)

        if current.right is not None:
            q.append(current.right)


# Tim kiem trong Binary Tree thuong
def search(root, value):
    if root is None:
        return False

    if root.data == value:
        return True

    return (search(root.left, value) or
            search(root.right, value))


# Dem so node
def count_nodes(root):
    if root is None:
        return 0

    return (1 + count_nodes(root.left)
              + count_nodes(root.right))


# Tao cay
root = Node(10)

root.left = Node(5)
root.right = Node(20)

root.left.left = Node(3)
root.left.right = Node(7)

root.right.left = Node(15)
root.right.right = Node(30)


print("Preorder:", end=" ")
preorder(root)

print("\nInorder:", end=" ")
inorder(root)

print("\nPostorder:", end=" ")
postorder(root)

print("\nLevel-order:", end=" ")
level_order(root)

print("\nNumber of nodes:", count_nodes(root))

value = 15

if search(root, value):
    print("Found", value)
else:
    print("Not found")