def counting_sort(T):
    maxVal = max(T)
    n = len(T)
    result = [0] * n
    count = [0] * (maxVal + 1)

    for i in range(n): 
        count[T[i]] += 1 

    for j in range(1, maxVal + 1): 
        count[j] += count[j-1]

    for i in range(n - 1, -1, -1):
        result[count[T[i]] - 1] = T[i]
        count[T[i]] -= 1 

    return result 

T = [1,4,8,2,4,0,1,5,6,7,3,2,9]
print(counting_sort(T))