def not_prime(n):
    if n <= 1:
        return False
    for i in range(2, int(n ** 0.5) + 1):
        if n % i == 0:
            return True
    return False

def task159(a,b,c):
    if a == 0 and b == 0:
        return int(not_prime(c))
    else:
        result = 0
        if a > 0:
            result += task159(a-1,b,c*2+1)
        if b > 0:
            result += task159(a, b-1, c*2)

        return result
    
