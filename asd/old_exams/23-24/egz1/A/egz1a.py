from egz1atesty import runtests
from queue import PriorityQueue

def armstrong( B, G, s, t):
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
  
  def edges_to_adjency_list(G):
    n = max(max(u,v) for u,v,_ in G) + 1

    adj = [[] for _ in range(n)]

    for u,v,w in G:
        adj[u].append((v,w))
        adj[v].append((u,w))
    return adj,n
  
  G = edges_to_adjency_list(G)[0]

  from_start = dijkstra(G, s)
  from_end = dijkstra(G, t)

  minTime = from_start[0][t]
  for bike in B:
     p = bike[1]
     q = bike[2]

     if p >= q:
        continue
     
     index = bike[0]

     currTime = from_start[0][index] + (from_end[0][index] * p / q)

     minTime = min(minTime, currTime)
     
  return minTime // 1

B = [(1, 1, 2), (2, 2, 3)]
G = [(0, 1, 6), (1, 4, 7), (4, 3, 4), (3, 2, 4), (2, 0, 3), (0, 3, 6)]
s = 0
t = 4
print(armstrong(B, G, s, t))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( armstrong, all_tests = True )
