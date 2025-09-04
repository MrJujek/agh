from egz1Atesty import runtests
import collections

def battle(P,K,R):
   m = len(P)
   n = len(K)
   field = [None] * (4*(m+n))

   for c in range(n):
      field[K[c]] = (K[c], R[c])
   for p in range(m):
      field[P[p]] = P[p]
   
   catapults = collections.deque()
   ans = 0

   for pos in range(len(field)):
      if field[pos] == None:
         continue

      if isinstance(field[pos], int) and len(catapults) == 0:
         continue

      if isinstance(field[pos], int) and len(catapults) != 0:
         while catapults:
            last = catapults.pop()
            if last[0] + last[1] >= pos:
               ans += 1
               break
         
         continue

      if isinstance(field[pos], tuple):
         catapults.append(field[pos])
         continue

   return ans

P = [14, 16, 0, 6, 10, 8]
K = [2, 12, 4]
R = [8, 5, 3]
# print(battle(P,K,R))
# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( battle, all_tests=True )
