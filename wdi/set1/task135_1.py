from random import randint

N = int(input("N:"))
T = [[randint(0,2) for _ in range(N)] for _ in range(N)]
print(*T, sep='\n')

def znajdz_trase(T, w, k, suma):
    # print("w:", w,"k:",k, "suma:", suma)
    if w == N - 1:
        return suma
    koszt = float("inf")
    for i in [-1,0,1]:
        if k + i > 0 and k + i < N:
            koszt = min(koszt, znajdz_trase(T, w + 1, k + i, suma + T[w+1][k]))
            # print("\t", koszt)

    return koszt


def start(T, k):
    koszt = T[0][k]
    return znajdz_trase(T, 0, k, koszt)

print(start(T, 5))