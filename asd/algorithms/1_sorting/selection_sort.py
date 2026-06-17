def array_selection_sort(T):
    for i in range(0, len(T)):
        small = i
        for j in range(i, len(T)):
            if T[j] < T[small]:
                small = j
            
        T[i], T[small] = T[small], T[i]

    return T

# class Node():
#     def __init__(self, val, next=None):
#         self.val = val
#         self.next = next
        
# def cut_node(head, to_cut):


# p = Node(5, Node(2, Node(1, Node(4, Node(3)))))
# w = p.next
# print(cut_node())
# # def node_selection_sort(head):


print(array_selection_sort([99,-1,-12,12,1,22,10,3,2,4,-1,0,0,0,0]))