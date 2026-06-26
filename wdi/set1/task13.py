def zad13():
    for i in range(1,290):
        dzielniki = []
        for j in range(1, i):
            if i % j == 0:
                dzielniki.append(j)

        if sum(dzielniki) == i:
            print(i)

zad13()