from egz2atesty import runtests

def wired(T):
  n = len(T)
  if n == 0:
    return 0
  
  memo = [[-1] * n for _ in range(n)]

  def solve(i, j):
    if i >= j:
      return 0

    if memo[i][j] != -1:
      return memo[i][j]
    
    minCost = float("inf")
    for k in range(i + 1, j + 1, 2):
      curr = 1 + abs(T[i] - T[k]) + solve(i + 1, k - 1) + solve(k + 1, j)
      minCost = min(minCost, curr)
    
    memo[i][j] = minCost
    return memo[i][j]
  
  return solve(0, n - 1)

# T = [7, 1, 3, 7, 2, 1]
# print(wired(T))
# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( wired, all_tests = True )
