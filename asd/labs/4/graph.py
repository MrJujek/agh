from collections import deque

def dwudzielny(G):
    n = len(G)
    Q = deque()
    color = [0 for _ in range(n)]
    color[0] = 1
    Q.append(0)
    
    while len(Q) > 0:
        u = Q.popleft()
        for i in range(n):
            if G[u][i] == 1:
                if color[i] == 0:
                    color[i] = -1 * color[u]
                    Q.append(i) 
            elif color[i] == color[u]: return False
            
def spojne_sklad(G):
    n = len(G)
    cnt = 0
    visited = [False] * n
    
    def DFSVisit(G, u):
        visited[u] = True
        for v in G[u]:
            if not visited[v]:
                DFSVisit(G, v)
                
    for i in range(n):
        if not visited[i]:
            cnt += 1
            DFSVisit(G, i)
        
    return cnt