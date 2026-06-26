S = float(input("S="))

eps = 1e-6
an = 0
an1 = 1

while abs(an - an1) > eps:
    an = an1
    an1 = ((S/an)+an)/2.0

print(an)
