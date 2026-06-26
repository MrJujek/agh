n = int(input("n="))

podzielniki = []

for i in range(1, n+1):
    if n % i == 0:
        podzielniki.append(i)

print(podzielniki)