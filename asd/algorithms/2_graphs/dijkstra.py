from queue import PriorityQueue
import heapq

def dijkstra(G, s): # reprezentacja sąsiedztwa, graf ważony
    n = len(G)
    distance = [float("inf")] * n
    parents = [None] * n
    visited = [False] * n
    distance[s] = 0

    queue = PriorityQueue()
    queue.put((distance[s], s))

    q = []
    heapq.heappush(q, (distance[s], s))

    while not queue.empty():
    # while q:
        dist, v = queue.get()
        # dist, v = heapq.heappop(q)

        # if dist > distance[v]:
        #     continue

        if not visited[v]:
            visited[v] = True

            for u, w in G[v]:
                if distance[u] > distance[v] + w:
                    distance[u] = distance[v] + w
                    parents[u] = v
                    queue.put((distance[u], u))
                    # heapq.heappush(q, (distance[u], u))

    return distance, parents

G = [
    [(2, 10), (3, 3)],
    [(3, 1), (4, 2)],
    [(2, 4), (4, 8), (1, 2)],
    [(1, 7)],
    [(2, 9)]
]
print(dijkstra(G, 0))
# E log V or V^2 log V