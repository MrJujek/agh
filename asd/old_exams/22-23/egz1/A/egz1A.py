from egz1Atesty import runtests
from queue import PriorityQueue

def gold(G,V,s,t,r):
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
  
  from_start = dijkstra(G, s)

  for i in range(len(G)):
     for j in range(len(G[i])):
        G[i][j] = (G[i][j][0], (G[i][j][1] * 2) + r)
  
  from_end = dijkstra(G, t)

  minimum = from_start[0][t]
  for i in range(len(V)):
    curr = -V[i] + from_start[0][i] + from_end[0][i]
    if curr < minimum:
        minimum = curr

  return minimum
  
G=[[(1, 9), (2, 2)], [(0, 9), (3, 2), (4, 6)], [(0, 2), (3, 7), (5, 1)], [(1, 2), (2, 7), (4, 2), (5, 3)], [(1, 6), (3, 2), (6,1)], [(2,1), (3,3), (6,8)], [(4,1), (5,8)]]
V=[25, 30, 20, 15, 5, 10, 0]
s=0 #start
t=6 #koniec
r=7 #łapówka
# print(gold(G, V, s, t, r))
# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( gold, all_tests = True )
