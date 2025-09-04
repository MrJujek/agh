def dfs(graph):
    n = len(graph)

    time = 0
    parent = [None] * n
    visited = [False] * n
    discovery = [float("inf")] * n
    processed = [float("inf")] * n

    def dfs_visit(v):
        nonlocal time

        # Increment time at visit
        time += 1
        discovery[v] = time
        visited[v] = True

        for u in graph[v]:
            if not visited[u]:
                parent[u] = v
                dfs_visit(u)

        # Increment time at backtrack
        time += 1
        processed[v] = time

    for v in range(n):
        if not visited[v]:
            dfs_visit(v)

    return visited, parent, discovery, processed

# E + V

G = [
    [1, 4],
    [2],
    [3],
    [],
    [2]
]
print(dfs(G))