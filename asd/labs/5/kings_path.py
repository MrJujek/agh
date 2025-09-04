# Dana jest szachownica o wymiarach n × n. Każde pole (i, j) ma koszt (liczbę ze zbioru {1, ..., 5})
# umieszczony w tablicy A (na polu A[j][i]). W lewym górnym rogu szachownicy stoi król, którego zadaniem
# jest przejsc do prawego dolnego rogu, przechodzac po polach o minmalnym sumarycznym koszcie. Prosze
# zaimplementowac funkcje kings_path(A), która oblicza koszt sciezki króla. Funkcja powinna byc mozliwie
# jak najszybsza.
from queue import PriorityQueue

def kings_path(T):
    n = len(T)
    
    queue = PriorityQueue()
    queue.put(( T[0][0],0, 0))
    
    visited = [[False] * n for _ in range(n)]
    
    cost = [[float("inf")] * n for _ in range(n)]
    cost[0][0] = T[0][0]

    while not queue.empty():
        curr, row, col = queue.get()
        visited[row][col] = True
        
        new_row = [row + 1, row + 1, row + 1, row, row, row - 1, row - 1, row - 1]
        new_col = [col + 1, col, col - 1, col + 1, col - 1, col + 1, col, col - 1]
        
        for i in range(8):
            if n > new_row[i] >= 0 and n > new_col[i] >= 0:
                if not visited[new_row[i]][new_col[i]]:
                    if cost[new_row[i]][new_col[i]] > cost[row][col] + T[new_row[i]][new_col[i]]:
                        cost[new_row[i]][new_col[i]] = cost[row][col] + T[new_row[i]][new_col[i]]
                        queue.put((cost[new_row[i]][new_col[i]],new_row[i], new_col[i]))
        
    print(cost)
    return cost[-1][-1]
        
        
        
T = [[3, 9, 0, 7],
     [9, 0, 1, 0],
     [9, 2, 5, 0],
     [9, 9, 9, 3]]
print(kings_path(T))