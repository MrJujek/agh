def counting_sort(arr, key_func):
    N = len(arr)
    count = [0] * N

    for num in arr:
        digit = key_func(num)
        count[digit] += 1

    for i in range(1, N):
        count[i] += count[i - 1]

    sorted_array = [0] * N

    for num in arr:
        digit = key_func(num)
        count[digit] -= 1
        sorted_array[count[digit]] = num

    return sorted_array

def sort(arr):
    N = len(arr)

    arr = counting_sort(arr, lambda x: x % N)

    arr = counting_sort(arr, lambda x: x // N)

    return arr