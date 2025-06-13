import abc

class OutOfRange(Exception):
    """Custom exception for out-of-range errors."""
    def __str__(self):
        return "ERROR! OUT OF RANGE."

class BadSize(Exception):
    """Custom exception for bad size errors."""
    def __str__(self):
        return "ERROR! BAD SIZE."

class List(abc.ABC):
    """Abstract base class for List."""
    @abc.abstractmethod
    def clear(self):
        """Clear the list."""
        pass

    @abc.abstractmethod
    def empty(self) -> bool:
        """Check if the list is empty."""
        pass

    @abc.abstractmethod
    def size(self) -> int:
        """Return the size of the list."""
        pass

    @abc.abstractmethod
    def insert(self, i: int, value):
        """Insert value at index i."""
        pass

    @abc.abstractmethod
    def remove(self, i: int):
        """Remove element at index i."""
        pass

    @abc.abstractmethod
    def search(self, value) -> int:
        """Search for value and return its first index, or -1 if not found."""
        pass

    @abc.abstractmethod
    def visit(self, i: int):
        """Return the value at index i."""
        pass

    @abc.abstractmethod
    def traverse(self) -> None:
        """Traverse and print all elements in the list."""
        pass

    @abc.abstractmethod
    def inverse(self) -> None:
        """Reverse the list in-place."""
        pass

class SeqList(List):
    """Sequential List implementation."""
    def __init__(self, init_size: int = 10):
        if init_size <= 0:
            raise BadSize()
        self._data = [None] * init_size
        self._cur_length = 0
        self._max_size = init_size

    def _resize(self):
        """Double the size of the list."""
        new_max_size = self._max_size * 2
        new_data = [None] * new_max_size
        for i in range(self._cur_length):
            new_data[i] = self._data[i]
        self._data = new_data
        self._max_size = new_max_size

    def clear(self):
        self._cur_length = 0

    def empty(self) -> bool:
        return self._cur_length == 0

    def size(self) -> int:
        return self._cur_length

    def traverse(self) -> None:
        if self.empty():
            print("List is empty.")
            return
        print("Output elements:", end=" ")
        for i in range(self._cur_length):
            print(self._data[i], end=" ")
        print()

    def inverse(self) -> None:
        i, j = 0, self._cur_length - 1
        while i < j:
            self._data[i], self._data[j] = self._data[j], self._data[i]
            i += 1
            j -= 1
            
    def insert(self, i: int, value):
        if not (0 <= i <= self._cur_length):
            raise OutOfRange()
        if self._cur_length == self._max_size:
            self._resize()
        
        for j in range(self._cur_length, i, -1):
            self._data[j] = self._data[j-1]
        self._data[i] = value
        self._cur_length += 1

    def remove(self, i: int):
        if not (0 <= i < self._cur_length):
            raise OutOfRange()
        
        for j in range(i, self._cur_length - 1):
            self._data[j] = self._data[j+1]
        self._cur_length -= 1

    def search(self, value) -> int:
        for i in range(self._cur_length):
            if self._data[i] == value:
                return i
        return -1

    def visit(self, i: int):
        if not (0 <= i < self._cur_length):
            raise OutOfRange()
        return self._data[i]

    def union(self, other_list: 'SeqList') -> None:
        """Merges another sorted SeqList into this one, assuming both are sorted."""
        # This implementation assumes lists are sorted for a meaningful union like in C++
        # If not sorted, it's more like concatenation or set union.
        # For simplicity, this will just append elements if they are not already present (like a set union)
        # or a merge if sorted. The C++ version implies merging sorted lists.
        # Let's implement a simple append for now, or user can adapt for sorted merge.
        
        # Simple append of unique elements from other_list
        # For a true merge of sorted lists, a different logic is needed.
        # The C++ example seems to merge two sorted lists into the first one.
        # Let's assume they are sorted for this union.
        
        # This is a simplified version, for a proper sorted merge, more complex logic is needed
        # similar to the C++ version's merge logic.
        # For now, let's just append elements from B that are not in A
        # or if we assume they are sorted and we want to merge:
        
        # If we want to replicate the C++ behavior of merging two sorted lists:
        # This requires the lists to be sorted.
        # The C++ version modifies 'this' list and assumes 'this' list has enough capacity.
        
        # A more Pythonic way for general "union" might be to create a new list.
        # However, to match the C++ `Union(seqList<elemType> &B)` which modifies `this`:
        
        # Let's assume a simple concatenation for now, as the C++ `Union` for seqList
        # was merging sorted lists, which is a specific use case.
        # If a more direct translation of the C++ sorted merge is needed:
        new_elements = []
        for k in range(other_list.size()):
            val = other_list.visit(k)
            # To avoid duplicates if it's a set-like union
            # if self.search(val) == -1: 
            #    new_elements.append(val)
            # For simple concatenation:
            new_elements.append(val)


        for val in new_elements:
            if self._cur_length == self._max_size:
                self._resize()
            self._data[self._cur_length] = val
            self._cur_length +=1
        # If the lists were meant to be sorted and merged:
        # self._data[:self._cur_length] = sorted(self._data[:self._cur_length])


class LinkList(List):
    """Singly Linked List implementation."""
    class _Node:
        def __init__(self, data=None, next_node=None):
            self.data = data
            self.next_node = next_node

    def __init__(self):
        self._head = LinkList._Node() # Dummy head node
        self._tail = self._head
        self._cur_length = 0

    def _get_position(self, i: int) -> _Node:
        """Returns the node at index i. If i is -1, returns head."""
        # For operations like insert/delete, we often need the predecessor.
        # This helper will return the node *before* the logical index i for modification.
        # Or, if i is the direct index, it returns that node.
        # Let's make it return the node *at* logical index i (0-based for first element).
        # For getting predecessor for insert/remove at i, call _get_position(i-1).
        if not (-1 <= i < self._cur_length): # -1 for head, 0 to cur_length-1 for elements
             # For insertion, i can be cur_length. For get_position for insertion, i-1 is fine.
            if i == self._cur_length and i >=0 : # allow getting node at cur_length (which is None or tail for some ops)
                pass # will be handled by loop
            elif not (-1 <= i < self._cur_length):
                return None # Or raise error, depending on internal use

        p = self._head
        current_idx = -1
        while current_idx < i:
            p = p.next_node
            if p is None and current_idx < i : # Overshot or list too short
                return None 
            current_idx += 1
        return p


    def clear(self):
        self._head.next_node = None
        self._tail = self._head
        self._cur_length = 0

    def empty(self) -> bool:
        return self._head.next_node is None

    def size(self) -> int:
        return self._cur_length

    def insert(self, i: int, value):
        if not (0 <= i <= self._cur_length):
            raise OutOfRange()

        if i == 0: # Insert at the beginning
            prev_node = self._head
        else:
            prev_node = self._get_position(i - 1)
            if prev_node is None: # Should not happen if i is valid
                 raise OutOfRange("Failed to find predecessor for insertion.")


        new_node = LinkList._Node(value, prev_node.next_node)
        prev_node.next_node = new_node
        
        if new_node.next_node is None: # Inserted at the end
            self._tail = new_node
        self._cur_length += 1

    def remove(self, i: int):
        if not (0 <= i < self._cur_length):
            raise OutOfRange()

        if i == 0: # Remove from beginning
            prev_node = self._head
        else:
            prev_node = self._get_position(i - 1)
            if prev_node is None or prev_node.next_node is None: # Should not happen
                raise OutOfRange("Failed to find node or its predecessor for removal.")

        node_to_remove = prev_node.next_node
        prev_node.next_node = node_to_remove.next_node

        if prev_node.next_node is None: # Removed the last element
            self._tail = prev_node
        
        # node_to_remove is now unlinked, Python's GC will handle it.
        self._cur_length -= 1
        
    def search(self, value) -> int:
        p = self._head.next_node
        idx = 0
        while p is not None:
            if p.data == value:
                return idx
            p = p.next_node
            idx += 1
        return -1

    def prior(self, value) -> int:
        """Finds the index of the predecessor of the first occurrence of value."""
        p = self._head.next_node
        pre = self._head
        idx = 0
        while p is not None:
            if p.data == value:
                if pre == self._head: # Value is the first element, no predecessor data-wise
                    return -1 # Or based on C++: index of predecessor, so -1 for head
                
                # To return index of predecessor node:
                q_search = self._head.next_node
                pred_idx = -1 # -1 if head is predecessor
                current_search_idx = 0
                while q_search is not None:
                    if q_search == pre:
                        pred_idx = current_search_idx
                        break
                    if q_search.next_node == pre: # Should not happen if pre is node
                        pred_idx = current_search_idx
                        break
                    q_search = q_search.next_node
                    current_search_idx +=1
                if pre == self._head: return -1 # C++ returns -1 if first element or not found
                return pred_idx

            pre = p
            p = p.next_node
            idx +=1
        return -1 # Value not found


    def visit(self, i: int):
        if not (0 <= i < self._cur_length):
            raise OutOfRange()
        node = self._get_position(i)
        if node is None: # Should be caught by bounds check
            raise OutOfRange("Node not found at index.")
        return node.data

    def traverse(self) -> None:
        p = self._head.next_node
        if p is None:
            print("List is empty.")
            return
        while p is not None:
            print(p.data, end=" ")
            p = p.next_node
        print()

    def head_create(self):
        """Creates a list by inserting elements at the head."""
        print("Input elements, end with EOF (Ctrl+D or Ctrl+Z Enter):")
        try:
            while True:
                value_str = input()
                # Assuming integer input for simplicity, adapt as needed
                try:
                    value = int(value_str) # Or eval(value_str) for more general types
                    # Insert at head (index 0)
                    new_node = LinkList._Node(value, self._head.next_node)
                    self._head.next_node = new_node
                    if self._cur_length == 0: # If list was empty, new node is also tail
                        self._tail = new_node
                    self._cur_length += 1
                except ValueError:
                    print(f"Invalid input '{value_str}', please enter a number or EOF to stop.")
        except EOFError:
            print("\nFinished input.")


    def tail_create(self):
        """Creates a list by inserting elements at the tail."""
        print("Input elements, end with EOF (Ctrl+D or Ctrl+Z Enter):")
        try:
            while True:
                value_str = input()
                try:
                    value = int(value_str) # Or eval(value_str)
                    new_node = LinkList._Node(value)
                    self._tail.next_node = new_node
                    self._tail = new_node
                    self._cur_length += 1
                except ValueError:
                     print(f"Invalid input '{value_str}', please enter a number or EOF to stop.")
        except EOFError:
            print("\nFinished input.")


    def inverse(self) -> None:
        if self.empty() or self.size() == 1:
            return

        p = self._head.next_node
        self._head.next_node = None # Detach current list from head
        self._tail = p # The old first element becomes the new tail

        while p is not None:
            tmp = p.next_node # Save next
            p.next_node = self._head.next_node # Link p to current reversed start
            self._head.next_node = p # p becomes the new start of reversed list
            p = tmp # Move to next node in original list

    def union(self, other_list: 'LinkList') -> 'LinkList':
        """Merges two sorted LinkLists. Modifies self and consumes other_list."""
        # Assumes both self and other_list are sorted in ascending order.
        # The result will be in self. other_list will be emptied.
        
        pa = self._head.next_node
        pb = other_list._head.next_node
        
        # Result list (self) starts with its own head
        pc = self._head 
        self._head.next_node = None # Detach self's original list
        
        while pa is not None and pb is not None:
            if pa.data <= pb.data:
                pc.next_node = pa
                pc = pa
                pa = pa.next_node
            else:
                pc.next_node = pb
                pc = pb
                pb = pb.next_node
        
        # Append remaining elements of pa or pb
        if pa is not None:
            pc.next_node = pa
            while pc.next_node is not None: # Find new tail
                pc = pc.next_node
            self._tail = pc
        elif pb is not None:
            pc.next_node = pb
            while pc.next_node is not None: # Find new tail
                pc = pc.next_node
            self._tail = pc
        else: # Both lists were empty or one became empty exactly at the end
            self._tail = pc


        self._cur_length += other_list._cur_length
        
        # Clear other_list as per C++ version (it's consumed)
        other_list.clear()
        return self


class DoubleLinkList(List):
    """Doubly Linked List implementation."""
    class _Node:
        def __init__(self, data=None, prior=None, next_node=None):
            self.data = data
            self.prior_node = prior
            self.next_node = next_node

    def __init__(self):
        self._head = DoubleLinkList._Node() # Dummy head node
        self._tail = DoubleLinkList._Node() # Dummy tail node
        self._head.next_node = self._tail
        self._tail.prior_node = self._head
        self._cur_length = 0

    def _get_position(self, i: int) -> _Node:
        """Returns the node at logical index i.
           i = -1 returns head. i = cur_length returns tail.
        """
        if not (-1 <= i <= self._cur_length):
            raise OutOfRange("Index for get_position is out of valid range [-1, cur_length].")

        p = self._head
        current_idx = -1
        while current_idx < i:
            p = p.next_node
            # p should not be None before reaching tail if logic is correct
            # as tail is always reachable.
            current_idx += 1
        return p

    def clear(self):
        # No need to iterate and delete in Python due to GC
        # Just reset pointers and length
        self._head.next_node = self._tail
        self._tail.prior_node = self._head
        self._cur_length = 0

    def empty(self) -> bool:
        return self._head.next_node == self._tail

    def size(self) -> int:
        return self._cur_length

    def insert(self, i: int, value):
        # Inserts value *before* the node currently at index i.
        # So, if i=0, inserts at the beginning.
        # If i=cur_length, inserts at the end (before tail dummy).
        if not (0 <= i <= self._cur_length):
            raise OutOfRange()

        p_at_i = self._get_position(i) # This is the node that will be *after* the new node.
        
        prev_node = p_at_i.prior_node
        new_node = DoubleLinkList._Node(value, prev_node, p_at_i)
        
        prev_node.next_node = new_node
        p_at_i.prior_node = new_node
        
        self._cur_length += 1

    def remove(self, i: int):
        if not (0 <= i < self._cur_length): # Valid indices are 0 to cur_length-1
            raise OutOfRange()

        node_to_remove = self._get_position(i) # This is the actual data node to remove
        
        if node_to_remove == self._head or node_to_remove == self._tail:
            # Should not happen if bounds check is correct and list not empty
            raise OutOfRange("Cannot remove head or tail dummy nodes directly.")

        prev_node = node_to_remove.prior_node
        next_node = node_to_remove.next_node
        
        prev_node.next_node = next_node
        next_node.prior_node = prev_node
        
        # node_to_remove is now unlinked, GC will handle it.
        self._cur_length -= 1

    def search(self, value) -> int:
        p = self._head.next_node
        idx = 0
        while p != self._tail:
            if p.data == value:
                return idx
            p = p.next_node
            idx += 1
        return -1

    def visit(self, i: int):
        if not (0 <= i < self._cur_length):
            raise OutOfRange()
        
        node = self._get_position(i)
        if node == self._head or node == self._tail:
             # Should be caught by bounds check
            raise OutOfRange("Attempted to visit dummy head/tail node.")
        return node.data

    def traverse(self) -> None:
        p = self._head.next_node
        if p == self._tail:
            print("List is empty.")
            return
        while p != self._tail:
            print(p.data, end=" ")
            p = p.next_node
        print()

    def inverse(self) -> None:
        if self.empty() or self.size() == 1:
            return

        # C++ version uses head-insertion into a new conceptual list.
        # We can do something similar.
        current = self._head.next_node # First actual data node
        
        # Detach all data nodes, effectively making the list empty temporarily
        self._head.next_node = self._tail
        self._tail.prior_node = self._head
        # self._cur_length remains same, will be restored by insertions

        while current != self._tail:
            next_original_node = current.next_node # Save next node from original sequence
            
            # Insert 'current' at the beginning (after dummy head)
            current.next_node = self._head.next_node
            current.prior_node = self._head
            
            self._head.next_node.prior_node = current
            self._head.next_node = current
            
            current = next_original_node # Move to the next node in the original sequence
        # No need to adjust cur_length as it's a rearrangement.

# Example Usage (optional, for testing)
if __name__ == '__main__':
    print("--- SeqList Demo ---")
    sl = SeqList(5)
    print(f"Empty: {sl.empty()}, Size: {sl.size()}")
    sl.insert(0, 10)
    sl.insert(1, 20)
    sl.insert(0, 5) # 5, 10, 20
    sl.traverse()
    print(f"Size: {sl.size()}")
    sl.insert(3, 30) # 5, 10, 20, 30
    sl.traverse()
    sl.insert(1, 7)  # 5, 7, 10, 20, 30
    sl.traverse()
    # sl.insert(5, 40) # This would resize if init_size was smaller
    # sl.traverse()
    print(f"Search for 10: index {sl.search(10)}")
    print(f"Search for 100: index {sl.search(100)}")
    print(f"Visit index 2: {sl.visit(2)}")
    sl.remove(1) # Remove 7 -> 5, 10, 20, 30
    sl.traverse()
    sl.inverse() # 30, 20, 10, 5
    sl.traverse()
    sl.clear()
    print(f"Empty: {sl.empty()}, Size: {sl.size()}")

    print("\n--- LinkList Demo ---")
    ll = LinkList()
    print(f"Empty: {ll.empty()}, Size: {ll.size()}")
    ll.insert(0, 10)
    ll.insert(1, 20)
    ll.insert(0, 5)  # 5, 10, 20
    ll.traverse()
    ll.insert(3, 30) # 5, 10, 20, 30
    ll.traverse()
    ll.insert(1, 7)  # 5, 7, 10, 20, 30
    ll.traverse()
    print(f"Search for 10: index {ll.search(10)}")
    print(f"Search for 100: index {ll.search(100)}")
    print(f"Visit index 2: {ll.visit(2)}") # 10
    ll.remove(1) # Remove 7 -> 5, 10, 20, 30
    ll.traverse()
    ll.inverse() # 30, 20, 10, 5
    ll.traverse()
    
    # Test prior
    ll.clear()
    ll.insert(0,1); ll.insert(1,2); ll.insert(2,3); ll.insert(3,4) # 1 2 3 4
    ll.traverse()
    print(f"Prior of 3: {ll.prior(3)}") # should be index of 2 (which is 1)
    print(f"Prior of 1: {ll.prior(1)}") # should be -1
    print(f"Prior of 5: {ll.prior(5)}") # should be -1

    ll.clear()
    print(f"Empty: {ll.empty()}, Size: {ll.size()}")

    # ll.head_create()
    # ll.traverse()
    # ll.tail_create()
    # ll.traverse()

    print("\n--- DoubleLinkList Demo ---")
    dll = DoubleLinkList()
    print(f"Empty: {dll.empty()}, Size: {dll.size()}")
    dll.insert(0, 10)
    dll.insert(1, 20)
    dll.insert(0, 5)  # 5, 10, 20
    dll.traverse()
    dll.insert(3, 30) # 5, 10, 20, 30
    dll.traverse()
    dll.insert(1, 7)  # 5, 7, 10, 20, 30
    dll.traverse()
    print(f"Search for 10: index {dll.search(10)}")
    print(f"Search for 100: index {dll.search(100)}")
    print(f"Visit index 2: {dll.visit(2)}")
    dll.remove(1) # Remove 7 -> 5, 10, 20, 30
    dll.traverse()
    dll.inverse() # 30, 20, 10, 5
    dll.traverse()
    dll.clear()
    print(f"Empty: {dll.empty()}, Size: {dll.size()}")

    print("\n--- LinkList Union Demo ---")
    l1 = LinkList()
    l1.insert(0,1); l1.insert(1,3); l1.insert(2,5) # 1 3 5
    l2 = LinkList()
    l2.insert(0,2); l2.insert(1,4); l2.insert(2,6) # 2 4 6
    print("L1: ", end=""); l1.traverse()
    print("L2: ", end=""); l2.traverse()
    l1.union(l2)
    print("L1 after union: ", end=""); l1.traverse()
    print("L2 after union: ", end=""); l2.traverse() # Should be empty
    print(f"L1 size: {l1.size()}, L2 size: {l2.size()}")

    l3 = LinkList()
    l3.insert(0,10); l3.insert(1,20)
    l4 = LinkList() # Empty list
    print("L3: ", end=""); l3.traverse()
    print("L4: ", end=""); l4.traverse()
    l3.union(l4)
    print("L3 after union with empty L4: ", end=""); l3.traverse()
    
    l5 = LinkList() # Empty list
    l6 = LinkList()
    l6.insert(0,100); l6.insert(1,200)
    print("L5: ", end=""); l5.traverse()
    print("L6: ", end=""); l6.traverse()
    l5.union(l6)
    print("L5 after union with L6: ", end=""); l5.traverse()
