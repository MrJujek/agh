def floyd_warshall(G): # reprezentacja macierzowa, graf ważony
    n = len(G)

    dist = [[float("inf")] * n for _ in range(n)]
    parent = [[None] * n for _ in range(n)]

    for v in range(n):
        for u in range(n):
            dist[v][u] = G[v][u]

    for v in range(n):
        for u in range(n):
            if G[v][u] != float('inf') and v != u:
                parent[v][u] = v

    for k in range(n):
        for v in range(n):
            for u in range(n):
                if dist[v][k] + dist[k][u] < dist[v][u]:
                    dist[v][u] = dist[v][k] + dist[k][u]
                    parent[v][u] = parent[k][u]

    return parent, dist

# V^3