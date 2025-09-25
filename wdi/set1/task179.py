class Node():
    def __init__(self, val):
        self.val = val
        self.next = None

def print_list(p):
        while p != None:
            print(f'{p.val} -> ', end='')
            p = p.next
        print("None")

def bucket_sort(head):
    tfirst = [None for _ in range(10)]
    tlast = [None for _ in range(10)]

    p = head
    while p is not None:
        a = p.val % 10

        if tfirst[a] is None:
            tfirst[a] = tlast[a] = p
        else:
            tfirst[a].next = p
            tlast[a] = p

        p = p.next

    res = None
    for i in range(9, -1, -1):
        if tlast[i] is not None:
            tlast[i].next = res
            res = tfirst[i]

    return res

p = Node(2)
p.next = Node(3)
p.next.next = Node(5)
p.next.next.next = Node(7)
p.next.next.next.next = Node(4)
p.next.next.next.next.next = Node(12)

print_list(bucket_sort(p))
