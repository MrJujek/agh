n = int(input("podaj n:"))

seen = []

for a in range(3, n):
    for b in range(4, n):
        c = (a**2 + b**2)**(1/2)
        if c - int(c) == 0 and c not in seen and a + b > c and a + c > b and b + c > a and c < n:
            seen.append(c)
            print(f'a: {a}, b: {b}, c: {c}')