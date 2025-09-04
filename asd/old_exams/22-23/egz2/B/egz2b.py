from egz2btesty import runtests

def parking(X,Y):
  n = len(X)
  m = len(Y)

  dp = [[float("inf")] * m for _ in range(n)]
  dp[0][0] = abs(X[0] - Y[0])

  for j in range(1, m):
    dp[0][j] = min(abs(X[0] - Y[j]), dp[0][j - 1])

  for i in range(1, n):
    for j in range(1, m):
      d = abs(X[i] - Y[j])
      dp[i][j] = min(dp[i][j - 1], dp[i - 1][j - 1] + d)
  
  return dp[-1][-1]

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( parking, all_tests = True )
