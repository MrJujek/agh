from egz1btesty import runtests

def kstrong(T, k):
    n = len(T)
    if n == 0:
        return 0

    global_max = 0

    prev_dp = [0] * (k + 1)

    for i in range(n):
        current_dp = [0] * (k + 1)
        
        for r in range(k + 1):
            sum_if_kept = T[i] + max(0, prev_dp[r])

            sum_if_removed = -float("inf")
            if r > 0:
                sum_if_removed = prev_dp[r - 1]

            current_dp[r] = max(sum_if_kept, sum_if_removed)
            global_max = max(global_max, current_dp[r])
        
        prev_dp = current_dp

    return global_max
              
T = [-20, 5, -1, 10, 2, -8, 10]
# print(kstrong(T, 1))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( kstrong, all_tests = True )
