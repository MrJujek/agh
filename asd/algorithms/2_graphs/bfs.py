from collections import deque

def bfs(graph, s):
    n = len(graph)
    queue = deque()

    # Initialize state arrays.
    visited = [False] * n
    parent = [None] * n
    distance = [float("inf")] * n

    # Mark starting node as visited.
    visited[s] = True
    distance[s] = 0
    queue.appendleft(s)

    while queue:
        v = queue.pop()

        # For every neighbour of v.
        for u in graph[v]:
            # Skip it if it's already visited.
            if visited[u]:
                continue

            # Mark it as visited.
            visited[u] = True
            parent[u] = v
            distance[u] = distance[v] + 1
            queue.appendleft(u)

    return visited, distance, parent

# E + V

G = [
    [1, 4],
    [2],
    [3],
    [],
    [2]
]
s = 0
print(bfs(G, s))