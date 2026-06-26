def podzbiory(T, a=0, b=0, c=0, n=0):
    if n == len(T):
        return a == b == c
    
    return podzbiory(T, a + T[n], b, c, n+1)\
        or podzbiory(T, a, b + T[n], c, n+1)\
        or podzbiory(T, a, b, c + T[n], n+1)

print(podzbiory([2,3,5,7,15]))
print(podzbiory([5,7,15]))