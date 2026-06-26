N = 5
T = [[0 for _ in range(N)] for _ in range(N)]

def zad92(T):
    diff_y = 0
    diff_x = 0
    index_y = 0
    index_x = 0

    for i in range(1, 5*5 + 1):
        T[index_y + diff_y][index_x + diff_x] = i
        index_x += diff_x
        index_y += diff_y

        if diff_y == 0 and diff_x == 0:
            diff_x = 1
            diff_y = 0
        elif index_y + diff_y < 0 or (index_y + diff_y < N and index_x + diff_x < N and T[index_y + diff_y][index_x + diff_x] != 0):
            diff_x = 1
            diff_y = 0
        elif index_x + diff_x >= N or (index_y + diff_y < N and index_x + diff_x < N and T[index_y + diff_y][index_x + diff_x] != 0):
            diff_x = 0
            diff_y = 1
        elif index_y + diff_y >= N or (index_y + diff_y < N and index_x + diff_x < N and T[index_y + diff_y][index_x + diff_x] != 0):
            diff_x = -1
            diff_y = 0
        elif index_x + diff_x < 0 or (index_y + diff_y < N and index_x + diff_x < N and T[index_y + diff_y][index_x + diff_x] != 0):
            diff_x = 0
            diff_y = -1

    for i in range(N):
        print(T[i])

# zad92(T)

def zad92garek(T):
    licznik = 0
    l = len(T)
    p = 0
    k = l - 1
    while p <= k:
        for i in range(p, k+1): T[p][i]=licznik=licznik+1
        for i in range(p+1, k): T[i][k]=licznik=licznik+1
        for i in range(k,p,-1): T[k][i]=licznik=licznik+1
        for i in range(k,p,-1): T[i][p]=licznik=licznik+1
        p+=1
        k-=1
    # if l % 2 == 1:
    #     T[l//2][l//2] = licznik + 1

    for i in range(l):
        print(T[i])

zad92garek(T)