def has_same_numbers(x,y):
    arr = [0 for _ in range(10)]
    
    while x > 0:
        rest = x % 10
        arr[rest] += 1
        x = x //10

    while y > 0:
        rest = y % 10
        arr[rest] -= 1
        y = y //10

    for i in arr:
        if i != 0:
            return False
    
    return True

print(has_same_numbers(123,3212))