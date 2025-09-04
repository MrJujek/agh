def countsort(A, k): # k - 1 - max value in A
    n = len(A)
    C = [0] * k 
    B = [0] * n

    for i in range(n):
        C[A[i]] += 1

    for i in range(1, k):
        C[i] = C[i - 1] + C[i]

    for i in range(n - 1, -1, -1):
        C[A[i]] -= 1
        B[C[A[i]]] = A[i]

    for i in range(n):
        A[i] = B[i]

T = [5,4,0,2,1,3,2,3,2]
k = max(T)
print(T)
countsort(T, k + 1)
print(T)