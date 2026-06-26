n = int(input("n="))

ciag = []

a = b = 1
while a <= n:
    ciag.append(a)
    a,b=b,a+b

for i in range(1, len(ciag)):
    podciag = []
    for j in range(i, len(ciag)):
        podciag.append(ciag[j])
        if sum(podciag) == n:
            print(podciag)
