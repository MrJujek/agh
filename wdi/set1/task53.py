n = int(input("n="))

def wylicz(n):
    licznik = 0

    i2 = 1
    while i2 <= n:
        i3 = i2
        while i3 <= n:
            i5 = i3
            while i5 <= n:
                licznik += 1
                i5 *= 5
            i3 *= 3
        i2 *= 2
    
    return licznik

print(wylicz(n))

    