import time

# krotka = [(1, 2), (-1, 2), (-2, 1), (-2, -1), (-1, -2), (1, -2), (-1, 2)]
    # if w+krotka[i][0] >= 0\
    #     and w+krotka[i][0] < n\
    #     and k+krotka[i][1] >= 0\
    #     and k+krotka[i][1] < n\
    #     and T[w+krotka[i][0]][k+krotka[i][1]] == 0:

    #     return w+krotka[i][0], k+krotka[i][1]
    # return -1, -1


# def ruch2(T, w=0, k=0, c=1):
#     # wypisz(T)
#     T[w][k] = c
#     if c == n * n:
#         wypisz(T)
#     else:
#         for nw, nk in skok2(T, w, k):
#             ruch(T, nw, nk, c+1)
#         T[w][k] = 0

# def skok2(T, w, k):
#     dk = (1,2,2,1,-1,-2,-2,-1)
#     dw = (-2,-1,1,2,2,1,-1,-2)

#     for i in range(8):
#         nw, nk = w + dw[i], k + dk[i]

#         if  0 <= nw < n and\
#             0 <= nk < n and\
#             T[nw][nk] == 0:

#             yield nw,nk

n = 5
T = [[0 for _ in range(n)] for _ in range(n)]

def wypisz(T):
    for i in range(0, len(T)):
        for j in range(0, len(T[i])):
            print(f'{T[i][j]:3}', end="")
        print("")
    print("")

def skok(T, w, k, i):
    dk = (1,2,2,1,-1,-2,-2,-1)
    dw = (-2,-1,1,2,2,1,-1,-2)
    nw, nk = w + dw[i], k + dk[i]

    if  0 <= nw < n and\
        0 <= nk < n and\
        T[nw][nk] == 0:

        return nw,nk
    return -1,-1

def ruch(T, w=0, k=0, c=1):
    # wypisz(T)
    T[w][k] = c
    if c == n * n:
        wypisz(T)
    else:
        for i in range(8):
            a, b = skok(T,w,k,i)
            if a != -1:
                ruch(T, a, b, c+1)
                T[a][b] = 0

ruch(T)
