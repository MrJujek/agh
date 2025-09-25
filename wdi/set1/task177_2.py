class Node():
    def __init__(self, val):
        self.val = val
        self.next = None

def print_list(p):
        while p != None:
            print(f'{p.val} -> ', end='')
            p = p.next
        print("None")

def combine_lists(p, q):
    if p is None: return q
    if q is None: return p

    if p.val <= q.val:
        p.next = combine_lists(p.next, q)
        return p
    else:
        q.next = combine_lists(p, q.next)
        return q  

p = Node(2)
p.next = Node(3)
p.next.next = Node(5)
p.next.next.next = Node(7)
q = Node(2)
q.next = Node(4)
q.next.next = Node(8)

print_list(combine_lists(p, q))