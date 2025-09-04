# Julian Dworzycki
# Złożoność czasowa nlogn
# Złożoność pamięciowa nlogn
# Sortuję dane wykorzystując quickorta.
# Następnie przechodzę po posortowanej tablicy i sprawdzam czy między palikami zmieści się kombajn.
# Sprawdzam to odejmując od wiekszego palika mniejszy palik i sprawdzam czy odległość jest większa lub równa kombajnowi.
# Jeżeli kombajn się zmieści to zwiększam counter o jeden.
# Na koniec funkcja zwraca counter.

from kol1testy import runtests

def ogrodzenie(M, D, T):
  # def partition(A, l, r):
  #   x = A[r]
  #   i = l

  #   for j in range(l, r):
  #       if A[j] <= x:
  #           A[i], A[j] = A[j], A[i]
  #           i += 1

  #   A[i], A[r] = A[r], A[i]
  #   return i

  # def quicksort(A, l, r):
  #   while l < r:
  #       pivot = partition(A, l, r)
  #       quicksort(A, l, pivot - 1)
  #       l = pivot + 1

  def m_quicksort(A):
     if len(A) <= 1:
        return A
     pivot = A[len(A) // 2]
     left = []
     middle = []
     right = []
     for i in A:
        if i < pivot:
           left.append(i)
        elif i == pivot:
           middle.append(i)
        else:
           right.append(i)

     return m_quicksort(left) + middle + m_quicksort(right)

  # quicksort(T, 0, len(T) - 1)
  T = m_quicksort(T)
  count = 0
  for i in range(1, len(T)):
    if T[i] - T[i - 1] >= D:
      count += 1

  return count

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( ogrodzenie, all_tests = True )