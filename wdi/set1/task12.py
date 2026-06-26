n = int(input("n="))
k = 2

podzielniki = [1]

while n > 1:
    while n % k == 0:
        podzielniki.append(k)
        n /= k
    k += 1

print(podzielniki)