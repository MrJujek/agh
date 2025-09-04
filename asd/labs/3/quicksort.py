def partition(T, l , r):
    pivot = T[r]
    pointer = l - 1

    for i in range(l, r):
        if T[i] <= pivot:
            pointer += 1
            T[i], T[pointer] = T[pointer], T[i]

    T[r], T[pointer + 1] = T[pointer + 1], T[r]
    return pointer + 1

def quicksort(T, l, r):
    while l < r:
        pivot = partition(T, l, r)

        if pivot - l < r - pivot:
            quicksort(T, l, pivot - 1)
            l = pivot + 1
        else:
            quicksort(T, pivot + 1, r)
            r = pivot - 1


t = [1,4,2,7,2,9,0,3]
print(t)
quicksort(t, 0, len(t) - 1)
print(t)