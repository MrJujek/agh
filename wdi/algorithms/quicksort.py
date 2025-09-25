def func(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [ i for i in arr if i < pivot ]
    middle = [ i for i in arr if i == pivot ]
    right = [ i for i in arr if i > pivot ]

    return func(left)+ middle+func(right)

print(func([8,2,3,4,1,5,6,7]))