class Node():
    def __init__(self, val):
        self.val = val
        self.next = None

    def insert(head, el):
        prev = None
        p = head

        while p != None and p.val < el:
            prev, p = p, p.next

        if p != None and p.val == el:
            return
        
        new_list = Node(el)
        if prev != None:
            prev.next = new_list
            new_list.next = p
        else:
            new_list.next = head
            head = new_list
            
        return head

    def member(head, el):
        while head != None and head.val < el:
            head = head.next

        return head != None and head.val == el
    
    def print_list(self):
        p = self
        while p != None:
            print(f'{p.val} -> ', end='')
            p = p.next
        print("None")
    
x = Node(5)
x.print_list()
print(x.member(1))
x = x.insert(1)
x.print_list()
print(x.member(1))
x = x.insert(6)
x.print_list()
x = x.insert(3)
x.print_list()