class node:
    def __init__(self, value):
        self.value = value
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    def is_Empty(self):
        if self.head == None:
            return True
        return False

    #pushes value to front of the LL
    def push(self, value):
        head = node(value)
        if self.is_Empty():
            self.head = head
        else:
            head.next = self.head
            self.head = head

    def pop(self):
        out = node(self.head.value)
        self.head = self.head.next
        return out

    def reverse(self):
        previous = None
        current = self.head
        while current != None:
            next = current.next
            current.next = previous
            previous = current
            current = next
        self.head = previous

    def print_LL(self):
        current = self.head
        while current != None:
            print(current.value, end = " ")
            current = current.next
        print()
