def dziel(a, b, n):
    print(a//b, ".")

    for _ in range(n):
        a %= b
        a *= 10
        print(a//b, '')

dziel(29, 3, 5)