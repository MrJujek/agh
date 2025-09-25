import random
import math

W = 10 #wiersze
K = 10 #kolumny
T = [[random.randint(10,99) for _ in range(K)] for _ in range(W)]

for i in range(W):
    print(T[i])

def zad95(T):
    liczba_wierszy = len(T)
    liczba_kolumn = len(T[0])

    minimalna_suma_wiersza = 0
    suma_wiersza = 0
    for i in range(liczba_wierszy):
        for j in range(liczba_kolumn):
            suma_wiersza += T[i][j]
            minimalna_suma_wiersza = min(minimalna_suma_wiersza, suma_wiersza)



def zad95garek(T):
    W = len(T)
    K = len(T[0])
    sw, nw = math.inf, 0
    for y in range(W):
        s = 0
        for x in range(K):
            s += T[y][x]
        if s < sw:
            sw = s
            nw = y

    for y in range(W):
        s = 0
        for x in range(K):
            s += T[y][x]
        if s > sw:
            sw = s
            nw = y

    

def zad95garek_NxN(T):
    N = len(T)
    sw, nw = math.inf, 0
    sk, nk = 0, 0

    for y in range(N):
        s1 = s2 = 0
        for x in range(N):
            s1 += T[y][x]
            s2 += T[x][y]
        if s1 < sw:
            sw = s1
            nw = y
        if s2 > sk:
            sk = s2
            nk = y

    return nw, nk
    