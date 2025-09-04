def two_sum(T, x):
    T = sorted(T)
    l = 0
    r = len(T) - 1

    while l < r:
        if T[l] + T[r] == x:
            return l, r
        elif T[l] + T[r] < x:
            l += 1
        else:
            r -= 1
        
    return None

print(two_sum([1,3,4,5,8,10,17], 15))
