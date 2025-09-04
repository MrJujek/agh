def lomuto_partition(A, p, r):
    x = A[r]
    i = p - 1

    for j in range(p, r):
        if A[j] <= x:
            i += 1
            A[i], A[j] = A[j], A[i]
        
    A[i + 1], A[r] = A[r], A[i + 1]
    return i + 1

def quickselect(A, p, r, k):
    q = lomuto_partition(A, p, r)

    if q == k:
        return k
    elif k < q:
        return quickselect(A, p, q - 1, k)
    else:
        return quickselect(A, q + 1, r ,k)
    
def quicksort(A, p, r):
    if p < r:
        q = lomuto_partition(A, p, r)
        quicksort(A, p, q - 1)
        quicksort(A, q + 1, r)


T = [9,4,1,5,2,7,8,0,4,9,1,2,3]
print(T)
k = 6
print(T[quickselect(T, 0, len(T) - 1, k)])
quicksort(T, 0, len(T) - 1)
print(T[k])