def daj_czynniki(liczba):
    print("liczba",liczba)
    if liczba <= 1:
        return []
    
    czynniki = [False for _ in range(liczba+1)]

    dzielnik = 2
    while dzielnik <= liczba:
        if liczba % dzielnik == 0:
            czynniki[dzielnik] = True
        while liczba % dzielnik == 0:
            liczba //= dzielnik

        dzielnik += 1

    return czynniki

# n = int(input("n: "))
t = [4, 9, 10, 3, 5, 12, 2, 11, 12, 15212512]

def can_jump_to_the_end(t):
    l = len(t)
    can_jump = [False for _ in range(l)]
    can_jump[0] = True

    for i in range(l):
        print(i)
        if can_jump[i]:
            czynniki = daj_czynniki(t[i])
            print(czynniki)

            for j in range(len(czynniki)):
                if i + j < l and czynniki[j]:
                    can_jump[i + j]

    return can_jump[l-1]

print(t, can_jump_to_the_end(t))