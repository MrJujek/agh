from zad7testy import runtests


def orchard(T, m):
    n = len(T)

    dp = [[float("inf")] * m for _ in range(n + 1)]
    
    dp[0][0] = 0
    
    for i in range(1, n + 1):
        current_apples = T[i - 1]
        
        for j in range(m):
            dp[i][j] = dp[i-1][j] + 1
            
            prev_remainder = (j - current_apples % m + m) % m
            
            if dp[i-1][prev_remainder] != float("inf"):
                dp[i][j] = min(dp[i][j], dp[i-1][prev_remainder])

    return dp[n][0]

# 0 1 2 3 4 5 6
T = [2, 2, 7, 5, 1, 14, 7]
m = 7
# print(orchard(T, m))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(orchard, all_tests=True)
