class Node():
    def __init__(self, val, next=None):
        self.val = val
        self.next = next

def print_linked_list(head):
    q = head
    while q.next != None:
        print(q.val, end=" -> ")
        q = q.next
    print(q.val, "-> None")

def merge(p, q):
    if p.val < q.val:
        head = p
        result = p
        second = q
    else:
        head = q
        result = q
        second = p

    while head.next != None:
        if head.next.val <= second.val:
            head = head.next
        else:
            q = head.next
            head.next = second
            head = head.next
            second = q

    if second != None:
        head.next = second
        
    return result
            

l1 = Node(5, Node(9, Node(10, Node(12))))
l2 = Node(3, Node(6, Node(10, Node(13))))
print_linked_list(l1)
print_linked_list(l2)
print_linked_list(merge(l1, l2))