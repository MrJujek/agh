from queue import PriorityQueue

def dijkstra(G, s):
    n = len(G)
    distance = [float("inf")] * n
    parents = [None] * n
    visited = [False] * n
    queue = PriorityQueue()
    distance[s] = 0

    queue.put((distance[s], s))

    while not queue.empty():
        _dist, v = queue.get()

        if not visited[v]:
            visited[v] = True

            for u, w in G[v]:
                if distance[u] > distance[v] + w:
                    distance[u] = distance[v] + w
                    parents[u] = v
                    queue.put((distance[u], u))

    return distance, parents