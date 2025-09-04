def zad3(E, u, v, V):
    n = len(V)
    graph = [[] * n]
    dist = [float("inf")] * n
    dist[u] = 0

    for u, v, w in E:
        graph[u].append((v, w))
        graph[v].append((u, w))
        
         