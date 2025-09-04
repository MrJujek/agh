def matrix_to_edges(G): 
    n = len(G)

    edges = []
    for i in range(n): 

        for j in range(i +1, n): #Ten zakres gwarantuje, że rozpatrujemy 
            #tylko połowę macierzy (nad przekątną) — gdzie i < j

            if G[i][j] != 0: 
                edges.append((i,j,G[i][j]))
    return edges

def matrix_to_edges(G):
    n = len(G) 
    adj = []

    for i in range(n): 

        adj.append([])

    for i in range(n): 
        for j in range(n): 

            if G[i][j] == 1: 
                adj[i].append(j)
    return adj 

def list_to_edges(G):
    n =len(G)
    E = []
    for u in range(n):  
        for v, w in G[u]: 
            if u < v: 
                E.append((u,v,w))
    return E 

def edges_to_adjency_list(G):
    n = max(max(u,v) for u,v,_ in G) + 1

    adj = [[] for _ in range(n)]

    for u,v,w in G:
        adj[u].append((v,w))
        adj[v].append((u,w))
    return adj,n

def adj_to_matrix(G): 
    n = len(G)
    matrix = [[0 for _ in range(n)] for _ in range(n)]

    for i in range(n):

        for j, w in G[i]: 

            G[i][j] = w 
    return matrix 

def edges_to_matrix(G):
    n = max(max(u,v) for u,v,_ in G) + 1

    matrix = [[0 for _ in range(n)] for _ in range(n)]

    for u,v,w in G:
        matrix[u][v] = w
        matrix[v][u] = w
        
    return matrix