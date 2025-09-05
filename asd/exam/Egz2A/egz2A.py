# Julian Dworzycki
# Złożoność obliczeniowa: O(D(V+E))
# Tworzę zmienną successful_deliveries (którą zwrócę na koniec), która przechowuje liczbę udanych dostaw.
# Zamieniam krawędzie grafu na reprezntację sąsiedztwa.
# Każdego dnia sprawdzam czy jest dobra trasa dla kuriera.
# Wykorzystuję do tego funckję find_path_bfs która jest bfsem z modyfikacją że sprawdzam czy sąsiad nie jest zablokowany.
# Jak znalazłem trasę to zwracam True i zwiększam liczbę udanych dostaw.
# Na koniec zwracam liczbę udanych dostaw.

from egz2Atesty import runtests
from collections import deque

def kingnqueen( V,E,D,K,Q,B ):
  def find_path_bfs(adj, start, end, blocked):
    if start == end:
        return True

    queue = deque([start])
    visited = [False] * len(adj)
    visited[start] = True

    while queue:
        current_city = queue.popleft()

        if current_city == end:
            return True

        for neighbor in adj[current_city]:
            if not visited[neighbor] and neighbor != blocked:
                visited[neighbor] = True
                queue.append(neighbor)

    return False
  
  def edges_to_adjency_list(E, V):
    adj = [[] for _ in range(V)]

    for u,v in E:
        adj[u].append(v)
        adj[v].append(u)

    return adj
  
  
  succesful_deliveries = 0
  adj = edges_to_adjency_list(E, V)

  for i in range(D):
      king_city = K[i]
      queen_city = Q[i]
      blocked_city = B[i]

      if find_path_bfs(adj, king_city, queen_city, blocked_city):
          succesful_deliveries += 1

  return succesful_deliveries

V = 9
E = [(0, 1), (0, 2), (2, 1), (2, 3), (5, 2), (4, 2), (3, 4), (4, 5), (5, 6), (8, 6), (7, 8), (6, 7)]
D = 3
K = [0, 5, 7]
Q = [3, 6, 0]
B = [2, 2, 5]
# print( kingnqueen( V, E, D, K, Q, B ) )

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( kingnqueen, all_tests = True )
