import math
def is_prime(x):
    if x <= 1:
        return False
    i = 2
    while i*i <= x:
        if x % i == 0:
            return False
        i += 1
    return True

def is_palindrome(n):
    temp = 0
    k = n
    while k > 0:
        temp = temp * 10 + k % 10
        k //= 10

    return temp == n

def is_super_palindrome_prime(n):
    temp = n

    while temp > 10:
        if not is_prime(temp) or not is_palindrome(temp):
            return False
        temp //= 10
        temp %= 10**(int(math.log10(temp)))

    if temp == 0 and int(math.log10(n)+1)%2 == 0:
        return True
    
    return is_prime(temp)

def task44(n):
    counter = 0
    for i in range(2,n):
        if is_prime(i) and is_super_palindrome_prime(i):
            counter += 1
            # print(i)

    return counter

print(task44(131))