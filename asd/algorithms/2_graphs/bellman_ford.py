def bellman_ford(g, s): # lista sąsiedztwa, graf ważony
    n = len(g)

    def to_edges(g):
        e = []
        for v in range(len(g)):
            for u, cost in g[v]:
                e.append((v, u, cost))
        return e

    edges = to_edges(g)
    d = [float('inf') for _ in range(n)]
    parent = [None for _ in range(n)]
    d[s] = 0
    changed = False

    for _ in range(n - 1):
        changed = False
        for v, u, cost in edges:
            if d[u] > d[v] + cost:
                d[u] = d[v] + cost
                parent[u] = v
                changed = True
        if not changed:
            break

    if changed:
        for v, u, cost in edges:
            if d[u] > d[v] + cost:
                return None

    return parent, d

# V * E