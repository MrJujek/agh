def topological_sort(G, n):
    visited = [False] * n
    result = []

    def DFS(u):
        visited[u] = True

        for v, w in G[u]:
            if not visited[v]:
                DFS(v)

        result.append(u)

    for u in range(n):
        if not visited[u]:
            DFS(u)

    return result[::-1]

def shortest_path_from_s_to_all(G, s):
    n = len(G)
    distance = [float("inf")] * n
    parents = [None] * n
    order = topological_sort(G, n)
    distance[s] = 0

    for u in order:
        for v, w in G[u]:
            if distance[v] > distance[u] + w:
                distance[v] = distance[u] + w
                parents[v] = u

    return distance, parents

