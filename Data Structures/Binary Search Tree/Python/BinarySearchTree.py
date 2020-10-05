class TreeNode:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

class BST:
    def __init__(self):
        self.head = None

    #adds value into tree if key does not already exist
    def put(self, value, target = self.head):
        if target == None:
            self.head = node(value)
        if !self.contains(value):
            if self.head.value < value:
                self.put(value, self.head.left)
            else:
                self.put(value, self.head.right)

    def contains(self, value):
        if this.head.value == value:
            return True
        else:
            left = right = BST()
            left.head = self.head.left
            right.head = self.head.right
            return left.contains(value) or right.contains(value)
