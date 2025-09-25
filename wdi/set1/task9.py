n = int(input("n="))
a = 1
b = 1

iloczyn = a * b
while iloczyn < n:
    a,b = b,a+b
    iloczyn = a * b

if iloczyn == n:
    print(f'{n} jest wynikiem mnożenia {a} i {b}')
else:
    print(f'{n} nie da sie przedstawic jako iloczyn dwóch kolejnych liczb z ciagu fibbonaciego')
