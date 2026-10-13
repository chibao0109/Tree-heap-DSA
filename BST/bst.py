# =======================
# 1. Tạo Node
# =======================
class Node:
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


# =======================
# 2. INSERT
# =======================
def insert(root, x):
    # Cây con đang rỗng
    if root is None:
        return Node(x)

    # x nhỏ hơn root -> đi sang trái
    if x < root.val:
        root.left = insert(root.left, x)

    # x lớn hơn root -> đi sang phải
    elif x > root.val:
        root.right = insert(root.right, x)

    # x == root.val -> không thêm
    return root


# =======================
# 3. SEARCH
# =======================
def search(root, x):
    if root is None:
        return False

    if x == root.val:
        return True

    if x < root.val:
        return search(root.left, x)

    return search(root.right, x)


# =======================
# 4. FIND MIN
# =======================
def find_min(root):
    while root.left is not None:
        root = root.left
    return root


# =======================
# 5. FIND MAX
# =======================
def find_max(root):
    while root.right is not None:
        root = root.right
    return root


# =======================
# 6. DELETE
# =======================
def delete_node(root, x):
    # Không tìm thấy
    if root is None:
        return None

    # x nhỏ hơn root -> đi trái
    if x < root.val:
        root.left = delete_node(root.left, x)

    # x lớn hơn root -> đi phải
    elif x > root.val:
        root.right = delete_node(root.right, x)

    # Đã tìm thấy node cần xóa
    else:
        # Case 1: Không có con
        if root.left is None and root.right is None:
            return None

        # Case 2: Chỉ có con phải
        elif root.left is None:
            return root.right

        # Case 3: Chỉ có con trái
        elif root.right is None:
            return root.left

        # Case 4: Có cả hai con
        else:
            temp = find_min(root.right)
            root.val = temp.val
            root.right = delete_node(root.right, temp.val)

    return root


# =======================
# 7. INORDER
# Left -> Root -> Right
# =======================
def inorder(root, result):
    if root is None:
        return
    inorder(root.left, result)
    result.append(root.val)
    inorder(root.right, result)


# =======================
# 8. PREORDER
# Root -> Left -> Right
# =======================
def preorder(root, result):
    if root is None:
        return
    result.append(root.val)
    preorder(root.left, result)
    preorder(root.right, result)


# =======================
# 9. POSTORDER
# Left -> Right -> Root
# =======================
def postorder(root, result):
    if root is None:
        return
    postorder(root.left, result)
    postorder(root.right, result)
    result.append(root.val)


# =======================
# MAIN
# =======================
def main():
    root = None

    # Tạo BST
    for x in [8, 3, 10, 1, 6, 14, 4, 7]:
        root = insert(root, x)

    #         8
    #        / \
    #       3   10
    #      / \    \
    #     1   6    14
    #        / \
    #       4   7

    # Duyệt cây
    result = []
    inorder(root, result)
    print("Inorder:", " ".join(map(str, result)))

    result = []
    preorder(root, result)
    print("Preorder:", " ".join(map(str, result)))

    result = []
    postorder(root, result)
    print("Postorder:", " ".join(map(str, result)))

    # Search
    x = 6
    if search(root, x):
        print(f"{x} ton tai trong cay")
    else:
        print(f"{x} khong ton tai trong cay")

    # Min / Max
    print("Min:", find_min(root).val)
    print("Max:", find_max(root).val)

    # Delete
    root = delete_node(root, 3)

    result = []
    inorder(root, result)
    print("Inorder sau khi xoa 3:", " ".join(map(str, result)))


if __name__ == "__main__":
    main()
