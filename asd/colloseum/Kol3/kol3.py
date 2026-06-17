# Julian Dworzycki
# Złożoność obliczeniowa O(n + m)
# Funkcja sum_of_cut zwraca sumę liczb w kolumnie lub wierszu, który mielibyśmy wyciąć oraz zwraca ile komórek sklada się na tą sumę.
# Zaczynamy od lewego górnego pola i za pomocą funkcji wyżej sprawdzamy czy możemy coś wyciąć.
# Sprawdzamy co możemy wyciąć i przesuwamy odpowiednio indeks.
# Jeżeli możemy ściąć i wiersz i klumnę to wykonujemy rekurencyjne wywoławnie funkcjji które zwróci nam lepszy wybór (minimum).
# Jeżeli dotarliśmy do ostatniego pola to zwracamy odpowiedź.
from kol3testy import runtests

def parkiet(B, C, s):
    def sum_of_cut(C, dir, x, y):
        if dir == "col":
            if x + 1 >= len(C[0]):
                return float("inf"), -1
            
            return C[y][x] - C[y][x + 1], len(C) - y
        else:
            if y + 1 >= len(C):
                return float("inf"), -1
            
            return C[y][x] - C[y + 1][x], len(C[0]) - x

    n = len(B[0]) # x
    m = len(B) # y

    index_x = 0
    index_y = 0
        
    ans = 0

    # for i in range(m):
    #     for j in range(n):
    #         print(f'{i}, {j}: ', sum_of_cut(C, "col", j, i), sum_of_cut(C, "row", j, i), end="\t")
    #     print("")

    def cut(index_x, index_y, ans):
        while index_x < n or index_y < m:
            row_cut, row_cells = sum_of_cut(C, "row", index_x, index_y)
            col_cut, col_cells = sum_of_cut(C, "col", index_x, index_y)

            # if C[index_y][index_x] <= s:
            #     return ans

            if row_cut > s and col_cut > s:
                return -1
            
            if row_cut <= s and row_cut < col_cut:
                index_y += 1
                ans += 1
            elif col_cut <= s and col_cut < row_cut:
                index_x += 1
                ans += 1
            elif col_cut == row_cut <= s:
                return min(cut(index_x + 1, index_y, ans + 1), cut(index_x, index_y + 1, ans + 1))

            if (row_cut == float("inf") and col_cut <= s) or (col_cut == float("inf") and row_cut <= s):
                return ans

            if index_x == n - 1 and index_y == m - 1:
                return ans
        
        return ans
            
    
    ans = cut(index_x, index_y, ans)
    return ans

B = [(2,1,4),
     (1,3,1),
     (2,3,3)]
C = [(20,15,8),
     (13,10,4),
     (8,6,3)]

# B = [(10,10),
#      (10,10)]
# C = [(40,20),
#      (20,10)]

# B = [(1,1)]
# C = [(2,1)]
s = 5

# print("ans:", parkiet(B, C, s))
runtests(parkiet, all_tests = True)
