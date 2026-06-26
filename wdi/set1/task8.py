eps = 1e-6
a,b = 0, 128

while b - a > eps:
    x = (a + b) / 2 
    fx = (x ** x) / 2024

    if fx > 0:
        b = x
    else:
        a = x

wynik = (a+b)/2
print(wynik, wynik**wynik)