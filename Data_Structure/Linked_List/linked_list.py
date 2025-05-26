class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

class LinkedList:
    def __init__(self):
        self.head = None
    
    def add_at_head(self, val):
        new_node = ListNode(val)
        new_node.next = self.head
        self.head = new_node

    def add_at_tail(self, val):
        if not self.head:
            self.head = ListNode(val)
            return
        curr = self.head
        while curr.next:
            curr = curr.next
        curr.next = ListNode(val)
    
    def delete_node(self, val):
        curr = self.head
        while curr.next and curr.next.val != val:
            curr = curr.next
        if curr.next:
            curr.next = curr.next.next
    
    def update_node(self, old_val, new_val):
        curr = self.head
        while curr and curr.val != old_val:
            curr = curr.next
        if curr:
            curr.val = new_val
    
    def reverse(self):
        prev = None
        curr = self.head
        while curr:
            next_node = curr.next
            curr.next = prev
            prev = curr
            curr = next_node
        self.head = prev

    def print_list(self):
        curr = self.head
        while curr:
            print(f"{curr.val} -> ", end="")
            curr = curr.next
        print("NULL")

if __name__ == "__main__":
    lst = LinkedList()
    lst.add_at_tail(0)
    lst.add_at_tail(1)
    lst.add_at_tail(2)
    lst.print_list()
    lst.add_at_head(-1)
    lst.add_at_head(-2)
    lst.print_list()
    lst.delete_node(1)
    lst.print_list()
    lst.update_node(-2, 10)
    lst.print_list()
    lst.reverse()
    lst.print_list()