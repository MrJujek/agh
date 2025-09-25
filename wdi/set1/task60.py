import math

def nadwa(n, p):
    rozmiar = round(math.log2(n)) + 1

    chars = '0123456789ABCDEF'

    arr = [0 for _ in range(rozmiar)]

    index = 0
    while n > 0:
        arr[index] = n % p
        index += 1
        n //= p

    wynik = [0 for _ in range(index)]
    for j in range(index - 1, -1, -1):
        wynik[j] = chars[arr[j]]

    return wynik

print(nadwa(11, 2))

