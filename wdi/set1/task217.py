def find(p, x):
    if p is None:
        return False
    
    if p.val == x:
        return True
    
    if x > p.val:
        return find(p.right, x)
    else:
        return find(p.left, x)