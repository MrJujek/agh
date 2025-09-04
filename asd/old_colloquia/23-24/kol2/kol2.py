from kol2testy import runtests
from collections import deque

def warrior( G, s, t):
  def edges_to_adjency_list(G):
    n = max(max(u,v) for u,v,_ in G) + 1

    adj = [[] for _ in range(n)]

    for u,v,w in G:
      adj[u].append((v,w))
      adj[v].append((u,w))
      
    return adj,n
  
  def BFS(G, s, n):
    d = [[float("inf")] * 17 for _ in range(n)]
    q = deque()
    q.append((0,0,0,s))
    d[s][0] = 0

    while q:
      counter, dist, time_without_sleep, u = q.popleft()

      if counter > 0:
        q.append((counter - 1, dist, time_without_sleep, u))
        continue

      if dist > d[u][time_without_sleep]:
        continue

      for v, w in G[u]:
        if time_without_sleep + w <= 16:
          if d[v][time_without_sleep + w] > dist + w:
            d[v][time_without_sleep + w] = dist + w
            q.append((w-1, dist + w, time_without_sleep + w,v))
        else:
          if d[v][w] > d[u][time_without_sleep] + 8 + w:
            d[v][w] = d[u][time_without_sleep] + 8 + w
            q.append((w, d[v][w], w, v))

    return d

  G, n = edges_to_adjency_list(G)
  d = BFS(G, s, n)
  
  return min(d[t])

G = [ (1,5,10), (4,6,12), (3,2,8),
(2,4,4) , (2,0,10), (1,4,5),
(1,0,6) , (5,6,8) , (6,3,9)]
s = 0
t = 6
# print(warrior(G,s,t))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( warrior, all_tests = True )
