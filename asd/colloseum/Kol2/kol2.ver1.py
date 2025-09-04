from kol2testy import runtests
from queue import PriorityQueue

def lets_roll(start_city, flights, resorts):
    def edges_to_adjency_list(G):
        n = max(max(u,v) for u,v,_ in G) + 1

        adj = [[] for _ in range(n)]

        for u,v,w in G:
            adj[u].append((v,w))
            adj[v].append((u,w))

        return adj,n

    # def daj_wierzcholki(flights):
    #     wierzcholki = []
    #     for i in flights:
    #         if i[0] not in wierzcholki:
    #             wierzcholki.append(i[0])
    #         if i[1] not in wierzcholki:
    #             wierzcholki.append(i[1])
    #     return wierzcholki
    
    # wierzcholki = len(daj_wierzcholki(flights))
    adj, n = edges_to_adjency_list(flights)
    print(n)
    distance = [float("inf")] * n
    parents = [None] * n
    visited = [False] * n
    queue = PriorityQueue()
    distance[0] = 0

    queue.put((distance[0], start_city))

    while not queue.empty():
        _dist, v = queue.get()

        if not visited[v]:
            visited[v] = True

            for u, w in adj[v]:
                if distance[u] > distance[v] + w:
                    distance[u] = distance[v] + w
                    parents[u] = v
                    queue.put((distance[u], u))

    print(distance)
    return -1

start_city = 0
flights = [(0,1,2), (0,2,4), (0,3,8), (3,4,16), (1,4,1), (2,4,1)]
resorts = [1,2,4]
print(lets_roll(start_city, flights, resorts))

# runtests(lets_roll, all_tests = False)
