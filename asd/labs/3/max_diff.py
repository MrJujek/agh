def max_diff(T):
    n = len(T)
    if n < 2:
        return 0
    
    buckets = [[] for _ in range(n)]
    high, low = max(T), min(T)
    ran = high - low
    index = 0

    for num in T:
        if num == high:
            index = n - 1
        else:
            index = (num - low) * (n - 1) // (ran)
        
        buckets[index].append(num)

    res_buckets = []

    for i in range(n):
        if buckets[i]:
            res_buckets.append((min(buckets[i]), max(buckets[i])))

    maxi = 0
    result = ()

    for i in range(1, len(res_buckets)):
        temp = maxi
        maxi = max(maxi, res_buckets[i][0] - res_buckets[i - 1][1])

        if temp != maxi:
            result = (res_buckets[i - 1][1], res_buckets[i][0])

    return result

print(max_diff([20, 12,15,16,14,1,4,7,11]))