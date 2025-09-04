class Node:
    def __init__(self, val):
        self.right = None
        self.left = None
        self.val = val
        
    def insert(self, item):
        while True:
            if self.val >= item:
                if self.left is None:
                    self.left = Node(item)
                    return
                self = self.left
            else:
                if self.right is None:
                    self.right = Node(item)
                    return
                self = self.right
                
    def find(self, val):
        node = self
        while node.val != val:
            node = node.next   
        return node
                
    def next(self, val):
        node = self.find(self, val)
        
        if node.right is None:
            return None
        
        result = node.right
        
        while result.left:
            result = result.left
            
        return result.val