from random import randint

N = int(input("N:"))
T = [[randint(0,2) for _ in range(N)] for _ in range(N)]
print(*T, sep='\n')

tab = {}
def min_koszt(T,w,k):
    if (w, k) in tab:
        return tab[w, k]
    if k < 0 or k >= N:
        return float("inf")
    if w == N:
        return 0
    if w == N - 1:
        return T[w][k]
    tab[w, k] = min(min_koszt(T, w + 1, k-1), min_koszt(T, w + 1, k), min_koszt(T, w + 1, k+1)) + T[w][k]
    return tab[w, k]

print(min_koszt(T, 0, 5))