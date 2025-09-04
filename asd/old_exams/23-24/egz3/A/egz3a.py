from egz3atesty import runtests
import collections

def mykoryza( G,T,d ):
  n = len(G)
  mushroom = [float("inf")] * n

  q = collections.deque()
  for m in T:
    q.append(m)
    mushroom[m] = m

  count = 1

  while q:
    v = q.popleft()
    for u in G[v]:
      if mushroom[u] == float("inf"):
        mushroom[u] = mushroom[v]
        q.append(u)

        if mushroom[v] == T[d]:
          count += 1
          
  return count

G = [[1,3],[0,2,4],[1,5],
     [0,4,6],[1,3,5,7],[2,4,8],
     [3,7],[4,6,8],[7,5]]
T = [8,2,6]
d = 1
# print(mykoryza(G, T, d))
# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( mykoryza, all_tests = True )
