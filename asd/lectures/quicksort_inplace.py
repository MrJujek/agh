# Lomuto version
def lomuto_partition(A, p, r):
    x = A[r]
    i = p - 1

    for j in range(p, r):
        if A[j] <= x:
            i += 1
            A[i], A[j] = A[j], A[i]
        
    A[i + 1], A[r] = A[r], A[i + 1]
    return i + 1

# Hoare verion
def hoare_partition(A, p, r):
    x = A[p]
    a = p - 1
    b = r + 1

    while True:
        a += 1
        while A[a] < x:
            a += 1

        b -= 1
        while A[b] > x:
            b -= 1
        
        if a >= b:
            return b
        
        A[a], A[b] = A[b], A[a]

def quicksort(A, p, r):
    if p < r:
        # q = lomuto_partition(A, p, r)
        # quicksort(A, p, q - 1)
        # quicksort(A, q + 1, r)
        
        q = hoare_partition(A, p, r)
        quicksort(A, p, q)
        quicksort(A, q + 1, r)

def smarter_quicksort(A, p, r):
    while p < r:
        # q = lomuto_partition(A, p, r)
        # quicksort(A, p, q - 1)
        # p = q + 1
        
        q = hoare_partition(A, p, r)
        quicksort(A, p, q)
        p = q + 1

T = [9,4,1,5,2,7,8,0,4,9,1,2,3]
print(T)
quicksort(T, 0, len(T) - 1)
# smarter_quicksort(T, 0, len(T) - 1)
print(T)