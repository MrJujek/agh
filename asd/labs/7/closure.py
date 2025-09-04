def closure(G):
    n = len(G)
    R = [[0] * n for _ in range(n)]

    for k in range(n):
        for u in range(n):
            for v in range(n):
                if G[u][v] == 1 or (G[u][k] == 1 and G[k][v] == 1):
                    R[u][v] = 1

    return R