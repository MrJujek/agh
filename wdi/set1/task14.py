def dajLiczby() -> None:
    liczby = []

    for i in range(1,1_000):
        suma = 0
        for j in range(1, i):
            if i % j == 0:
                suma += j

        liczby.append(suma)

    return liczby

def zad15():
    liczby = dajLiczby()

    for index, i in enumerate(liczby):
        # print(index, index + 1, liczby[index])

        if index + 1 != liczby[index] and i < len(liczby) and index + 1 < len(liczby) and liczby[index + 1] < len(liczby) and liczby[liczby[index]-1] == index + 1:
            print(index + 1, i)

zad15()