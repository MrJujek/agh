n = int(input("n="))

for i in range(2, int(n**(1/2))):
    if n % i == 0:
        print(f'{n} nie jest liczbą pierwszą')
        break
else:
    print(f'{n} jest liczbą pierwszą')