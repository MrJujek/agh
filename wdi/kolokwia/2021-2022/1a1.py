def id_prime(liczba):
    if liczba <= 1:
        return False
    
    i = 2
    while i*i <= liczba:
        if liczba % i == 0:
            return False
        i+=1
    return True

k = int(input("k:"))

l = 1
while 10**l <= k:
    l+=1

tab = [0 for _ in range(l)]

for i in range(l - 1, -1, -1):
    tab[i] = k % 10
    k //= 10

ile = [0 for _ in range(10)]

najwieksza = 0

for i in range(len(tab) + 1):
    for j in range(i + 1, len(tab) + 1):
        liczba = tab[i:j]

        for z in liczba:
            ile[z] += 1
        
        rozne_cyfry = True

        for z in ile:
            if z > 1:
                rozne_cyfry = False

        ile = [0 for _ in range(10)]

        if rozne_cyfry:
            do_sprawdzenia = 0
            mnoznik = 1
            for z in range(len(liczba) - 1, -1, -1):
                do_sprawdzenia += liczba[z] * mnoznik
                mnoznik *= 10

            if id_prime(do_sprawdzenia):
                najwieksza = max(najwieksza, do_sprawdzenia)

print(najwieksza)
