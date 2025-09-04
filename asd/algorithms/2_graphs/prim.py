from queue import PriorityQueue

def prim(g, s):
    n = len(g)

    visited = [False for _ in range(n)]
    d = [float('inf') for _ in range(n)]
    parent = [None for _ in range(n)]

    d[s] = 0
    q = PriorityQueue()
    q.put((d[s], s))

    while not q.empty():
        _, u = q.get()
        if visited[u]:
            continue
        visited[u] = True
        for v, w in g[u]:
            if not visited[v] and d[v] > w:
                d[v] = w
                parent[v] = u
                q.put((w, v))

    t_w = 0

    for w in d:
        if w == float('inf'):
            # jeżeli graf był niespójny
            return None
        t_w += w

    return parent, t_w

# (V+E)logV