from kol1testy import runtests

def maxrank(T):
  count = 0
  maxi = 0

  for i in range(len(T)):
    count = 0
    for j in range(i):
      if T[j] < T[i]:
        count += 1
    
    maxi = max(maxi, count)
    
  return maxi

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( maxrank, all_tests = True )
