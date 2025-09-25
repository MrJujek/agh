def merge(A, left, mid, right):
    n1 = mid - left + 1
    n2 = right - mid

    L = [0] * n1
    R = [0] * n2

    for i in range(n1):
        L[i] = A[left + i]
    for j in range(n2):
        R[j] = A[mid + 1 + j]

    i = 0
    j = 0  
    k = left

    while i < n1 and j < n2:
        if L[i] <= R[j]:
            A[k] = L[i]
            i += 1
        else:
            A[k] = R[j]
            j += 1
        k += 1
    # print(L, R, A)
    
    while i < n1:
        A[k] = L[i]
        i += 1
        k += 1

    while j < n2:
        A[k] = R[j]
        j += 1
        k += 1

    # print(A)

def merge_sort(A, left, right):
    if left < right:
        mid = (left + right) // 2

        merge_sort(A, left, mid)
        merge_sort(A, mid + 1, right)
        merge(A, left, mid, right)

T = [5,8,4,5,1,8,6,4,3,6,0,8,1,3,6,7,2]
print(T)
merge_sort(T, 0, len(T) - 1)
print(T)