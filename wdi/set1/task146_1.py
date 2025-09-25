def skladniki(n, s=1, w=''):
    if n == 0 and '+' in w[:-1]:
        print(w[:-1])
    else:
        for i in range(s, n + 1):
            skladniki(n - i, i, w + str(i) + '+')

skladniki(4)

