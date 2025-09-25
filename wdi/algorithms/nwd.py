# def nwd(a, b):
#     dzielnik = odp = 2
#     while dzielnik <= a:
#         if a % dzielnik == 0 == b % dzielnik:
#             odp = dzielnik
#         dzielnik += 1
#     return odp

def nwd(a,b):
    while b != 0:
        a,b = b, a%b
    return a

print(nwd(75, 100))