class Node():
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def print_tree(p):
    if p == None:
        return
    
    print(p.val) # root
    print_tree(p.left) # left
    print_tree(p.right) # right

def count(p):
    if p is None:
        return 0
    
    return count(p.left) + 1 + count(p.right)

def size(p):
    if p is None:
        return 0
    
    counter = 0
    stack = [p]

    while len(stack) != 0:
        cur = stack.pop()
        counter += 1

        if cur.left != None:
            stack.append(cur.left)

        if cur.right != None:
            stack.append(cur.right)

    return counter

def count_leaves(p):
    if p is None:
        return 0
    if p.left is None and p.right is None:
        return 1
    
    return count_leaves(p.left) + count_leaves(p.right)

def count_nodes_on_n_level(p, n):
    if p is None:
        return 0
    
    if n == 0:
        return 1
    
    return count_nodes_on_n_level(p.left, n - 1) + count_nodes_on_n_level(p.right, n - 1)

def is_number_in_tree(p, number):
    if p is None:
        return False
    
    if p.val == number:
        return True
    
    return is_number_in_tree(p.left, number) or is_number_in_tree(p.right, number)

p = Node(5, Node(10, Node(20, Node(9)), Node(7)), Node(2, Node(4), Node(1)))
# print_tree(p)
# print(count(p))
# print(size(p))
# print(count_leaves(p))
print(count_nodes_on_n_level(p, 2))
print(is_number_in_tree(p, 20))