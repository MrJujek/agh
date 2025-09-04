from zad4testy import runtests
import heapq

def spacetravel( n, E, S, a, b ):
    def dijkstra(G, s):
        n = len(G)
        distance = [float("inf")] * n
        parents = [None] * n
        visited = [False] * n
        distance[s] = 0

        q = []
        heapq.heappush(q, (distance[s], s))

        while q:
            dist, v = heapq.heappop(q)

            if not visited[v]:
                visited[v] = True

                for u, w in G[v]:
                    if distance[u] > distance[v] + w:
                        distance[u] = distance[v] + w
                        parents[u] = v
                        heapq.heappush(q, (distance[u], u))

        return distance, parents
    
    def edges_to_adjency_list(G):
        n = max(max(u,v) for u,v,_ in G) + 1

        adj = [[] for _ in range(n)]

        for u,v,w in G:
            adj[u].append((v,w))
            adj[v].append((u,w))
        return adj,n
    
    G, n = edges_to_adjency_list(E)

    new_edges = []
    for i in S:
        new_edges.append((i, 0))
        G[i].append((n,0))
    
    G.append(new_edges)
    
    res = dijkstra(G, a)
    return res[0][b]

E = [(0,1, 5),
(1,2,21),
(1,3, 1),
(2,4, 7),
(3,4,13),
(3,5,16),
(4,6, 4),
(5,6, 1)]
S = [ 0, 2, 3 ]
a = 1
b = 5
n = 7
print(spacetravel(n, E, S, a, b))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( spacetravel, all_tests = True )
