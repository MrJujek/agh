def sito(x):
    T = [True for _ in range(x)]

    T[0] = T[1] = False

    for i in range(x):
        if T[i]:
            for j in range(2*i, x, i):
                T[j] = False

    return [i for i in range(x) if T[i]]

print(sito(100))
