def cut_tree(T):
    n = len(T)
    dp = [-float("inf")] * n

    for i in range(n):
        dp[i] = max(dp[i - 2] + T[i] if i - 2 >= 0 else T[i],
                    dp[i - 1] if i - 1 >= 0 else -float("inf"))

    print(dp)
    return dp[n - 1]

print(cut_tree([6,1,2,3,4,5]))