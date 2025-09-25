n = int(input("n="))

def moje(n):
    a = b = 1
    while a < n:
        a, b = b, a+b

        c = d = 1
        while c < n:
            c, d = d, c+d

            if a*c == n:
                return True
    return False

def is_fib(n, a, b):
    while a <= n:
        if a == n:
            return True
        
        a, b = b, a+b

    return False

def created_by_fib_numbers(n):
    a = b = 1

    while b*b <= n:
        if n % b == 0:
            if is_fib(n//b, a, b):
                return True
        a, b = b, a+b

    return False

print(created_by_fib_numbers(n))