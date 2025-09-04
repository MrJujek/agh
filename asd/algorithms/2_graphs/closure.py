def closure(G): # reprezentacja macierzowa
    n = len(G)
    R = [list(row) for row in G]

    for k in range(n):
        for u in range(n):
            for v in range(n):
                if R[u][v] == 1 or (R[u][k] == 1 and R[k][v] == 1):
                    R[u][v] = 1

    return R

# V^3 = n^3