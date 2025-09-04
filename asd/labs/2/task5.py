import math

# x, y | x, y
pojemniki = [(2,1,1,2),(4,2,2,3),(5,2,3,4),(1,4,2,2),(3,3,4,4)]


def sortuj_po_drugim_tuplu(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2][1]
    left = [ i for i in arr if i[1] < pivot ]
    middle = [ i for i in arr if i[1] == pivot ]
    right = [ i for i in arr if i[1] > pivot ] 

    return sortuj_po_drugim_tuplu(left)+ middle+sortuj_po_drugim_tuplu(right)

def zad(pojemniki, A):
    for i in range(len(pojemniki)):
        obj = (pojemniki[i][0] - pojemniki[i][2]) * (pojemniki[i][1] - pojemniki[i][3])
        pojemniki[i] = (abs(obj), pojemniki[i][3])

    # pojemniki = sorted(pojemniki, key=lambda x: x[1])
    pojemniki = sortuj_po_drugim_tuplu(pojemniki) 

    print(pojemniki)

    i = 0
    odp = 0
    while A > 0:
        print(A)
        if pojemniki[i][0] > A:
            i+=1
            break
        A -= pojemniki[i][0]
        i+=1
        odp += 1
        if i > len(pojemniki):
            return None
        
    return odp

print(zad(pojemniki, 7))