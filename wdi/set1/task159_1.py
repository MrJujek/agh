def not_prime(n):
    if n <= 1:
        return False
    for i in range(2, int(n ** 0.5) + 1):
        if n % i == 0:
            return True
    return False

count = 0
def task159(a,b,c):
    global count
    if a == 0 and b == 0:
        if not_prime(c):
            count += 1
    else:
        if a > 0:
            task159(a-1,b,c*2+1)
        if b > 0:
            task159(a, b-1, c*2)
    