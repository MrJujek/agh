# Złożoność czasowa n^2
# Złożoność pamięciowa n

from zadtesty import runtests
import heapq

def goodknight(G, s, t):
  n = len(G)

  adj = [[] for _ in range(n)]
  for i in range(n):
    for j in range(n):
      if G[i][j] != -1:
        adj[i].append((j, G[i][j]))  # (sąsiad, czas podróży)

  pq = [(0, s, 0)]  # (czas całkowity, obecny zamek, godziny od ostatniego noclegu)
  dist = [[float('inf')] * 17 for _ in range(n)]
  dist[s][0] = 0

  while pq:
    czas, zamek, godziny_od_noclegu = heapq.heappop(pq)

    if zamek == t:
      return czas

    for next_zamek, travel_time in adj[zamek]:
      new_godziny = godziny_od_noclegu + travel_time
      new_czas = czas + travel_time
  
      if new_godziny > 16: # nocleg
        new_godziny = travel_time
        new_czas += 8

      if new_czas < dist[next_zamek][new_godziny]:
        dist[next_zamek][new_godziny] = new_czas
        heapq.heappush(pq, (new_czas, next_zamek, new_godziny))

  return -1

# #       0  1  2  3  4  5
# G = [ [-1, 3, 8,-1,-1,-1 ], # 0
#       [ 3,-1, 3, 6,-1,-1 ], # 1
#       [ 8, 3,-1,-1, 5,-1 ], # 2
#       [-1, 6,-1,-1, 7, 8 ], # 3
#       [-1,-1, 5, 7,-1, 8 ], # 4
#       [-1,-1,-1, 8, 8,-1 ]] # 5
# s = 0
# t = 5
# print(goodknight(G, s, t))

runtests( goodknight, all_tests = True )