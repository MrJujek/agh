def calculate_e():
    e = 1
    wyraz = 1
    index = 1

    while wyraz > 0:
        e += wyraz
        index += 1
        wyraz /= index

    return e

print(calculate_e())