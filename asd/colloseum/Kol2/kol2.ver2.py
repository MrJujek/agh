# Julian Dworzycki
# Złożoność obliczeniowa: ElogV (wykorzystanie algorytmu Dijkstry)
# 1. Zamieniam sobie graf z reprezentacji krawędziowej na reprezentację listy sąsiedztwa
# (funkcja edges_to_adjency_list(G)).
# 2. Ustalam dystans do każdego wierzchołka na nieskończoność (*).
# Jako, że zawsze szukamy trasy do resortu o najniższym koście oraz nie chcemy lecieć przez resorty, w których już byliśmy
# to każda trasa do resortu będzie trasą, która w sobie nie ma żadnego resortu (poza tym docelowym).
# 3. W związku z tym wykorzystuję algorytm Dijkstry z tą zmianą, że jeżeli wierzchołek jest resortem
# to nie szukam ścieżki przez niego przechodzącej.
# Dostaję dzięki temu listę odległości do każdego wierzchołka, trasą nie przechodzącą przez resorty.
# 4. Ustalam zmienną answer na 0 (zmienna, którą będę zwracał).
# Następnie biorę każdy resort (z tablicy resorts) i jeżeli dystans do niego jest różny od nieskończoności(*)
# (co oznacza, że da się do niego dostać zgodnie z wytycznymi Grzesia).
# to do wyniku końcowego dodaję dwukrotność tego dystansu (lecimy tam i z powrotem).
# 5. Na koniec zwracam zmienną answer.

from kol2testy import runtests
from queue import PriorityQueue

def lets_roll(start_city, flights, resorts):
    def edges_to_adjency_list(G): # 1
        n = max(max(u,v) for u,v,_ in G) + 1

        adj = [[] for _ in range(n)]

        for u,v,w in G:
            adj[u].append((v,w))
            adj[v].append((u,w))

        return adj,n

    adj, n = edges_to_adjency_list(flights) # 1

    distance = [float("inf")] * n # 2 (*)
    parents = [None] * n
    visited = [False] * n
    queue = PriorityQueue()
    distance[start_city] = 0

    queue.put((distance[start_city], start_city))

    while not queue.empty(): # 3
        _dist, v = queue.get()

        if not visited[v] and v not in resorts:
            visited[v] = True

            for u, w in adj[v]:
                if distance[u] > distance[v] + w:
                    distance[u] = distance[v] + w
                    parents[u] = v
                    queue.put((distance[u], u))

    answer = 0 # 4
    for i in resorts:
        if distance[i] != float("inf"): # (*)
            answer += 2*(distance[i])

    return answer # 5

# start_city = 0
# flights = [(0,1,2), (0,2,4), (0,3,8), (3,4,16), (1,4,1), (2,4,1)]
# resorts = [1,2,4]
# print(lets_roll(start_city, flights, resorts))

runtests(lets_roll, all_tests = True)
