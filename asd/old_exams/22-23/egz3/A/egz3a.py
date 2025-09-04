from egz3atesty import runtests

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

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( goodknight, all_tests = True )
