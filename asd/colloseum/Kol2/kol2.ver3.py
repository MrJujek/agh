from kol2testy import runtests
from queue import PriorityQueue

def lets_roll(start_city, flights, resorts):
    def Dijkstra(graph, start):
        A = [None]*len(graph)
        q = PriorityQueue()
        q.put((0,start))

        while not q.empty():
            dist, node = q.get()
            if A[node] is None:  
                A[node] = dist
                for v in graph[node]:
                    if A[v[0]] is None and node not in resorts:
                        q.put((dist + v[1], v[0]))  
        return A
    
    def edges_to_adjency_list(G):
        n = max(max(u,v) for u,v,_ in G) + 1

        adj = [[] for _ in range(n)]

        for u,v,w in G:
            adj[u].append((v,w))
            adj[v].append((u,w))

        return adj,n

    adj, n = edges_to_adjency_list(flights)
    distance = Dijkstra(adj, start_city)

    answer = 0
    for i in resorts:
        if distance[i] != float("inf"):
            answer += 2*(distance[i])
    return answer

start_city = 0
flights = [(0,1,2), (0,2,4), (0,3,8), (3,4,16), (1,4,1), (2,4,1)]
resorts = [1,2,4]
# print(lets_roll(start_city, flights, resorts))

runtests(lets_roll, all_tests = True)
