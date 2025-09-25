def daj_dlugie_e(n):
    e = [0 for _ in range(n)]
    w = [0 for _ in range(n)]

    e[0] = 1
    w[0] = 1

    index = 1
    warunek = True

    while warunek:
        przeniesienie = 0
        warunek = False

        for i in range(n - 1, -1, -1):
            suma = e[i] + w[i] + przeniesienie
            e[i] = suma % 10
            przeniesienie = suma // 10

        index += 1

        reszta = 0
        for j in range(n):
            reszta = 10 * reszta + w[j]
            w[j] = reszta // index
            reszta %= index

            if w[j] > 0:
                warunek = True

    return e
 
print(daj_dlugie_e(10))
