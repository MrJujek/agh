class UF:
    def __init__(self, n):
        self.parent = [i for i in range(n)]
        self.rank = [0 for _ in range(n)]

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])

        return self.parent[x]

    def union(self, x, y):
        x = self.find(x)
        y = self.find(y)

        if x == y:
            return

        if self.rank[x] < self.rank[y]:
            self.parent[x] = y
        else:
            self.parent[y] = x

            if self.rank[x] == self.rank[y]:
                self.rank[x] += 1

def kruskal(g):
    n = len(g)

    def to_edges(g, n):
        edges = []
        for u in range(n):
            for v, cost in g[u]:
                if u < v:
                    edges.append((u, v, cost))
        return edges

    g_e = sorted(to_edges(g, n), key=lambda x: x[2])
    uf = UF(n)
    mst = []
    total_cost = 0
    mst_e = 0

    for u, v, cost in g_e:
        if mst_e >= n - 1:
            break
        if uf.find(u) != uf.find(v):
            uf.union(u, v)
            total_cost += cost
            mst.append((u, v, cost))
            mst_e += 1

    return mst, total_cost

# E log V lub E log E (bo sortowanie)