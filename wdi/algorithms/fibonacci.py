def fib(x):
    a = b = 1
    while a < x:
        print(a)
        a, b = b, a+b

fib(100)
    