def binary_search(T, w):
    l = 0
    r = len(T) - 1
    m = (l + r) // 2

    while l < r:
        if T[l] == w:
            return l
        elif T[r] == w:
            return r
        
        if T[m] > w:
            r = m
        elif T[m] < w:
            l = m
        else:
            return m
        
        m = (l + r) // 2

        if m == l or r == m:
            return None

    return m

print(binary_search([-1, 0,1,2,4,5,8], 0))