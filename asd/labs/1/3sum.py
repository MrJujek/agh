def two_sum(T, x):
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

def three_sum(T, x):
    T = sorted(T)

    for i in range(len(T)):
        res = two_sum(T[i:], x - T[i])
        if res:
            if T[i] + T[i:][res[0]] + T[i:][res[1]] == x:
                return i, res[0], res[1]
        
    return None

print(three_sum([1,3,4,5,8,10,17], 18))
