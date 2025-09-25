def czyfib(kom1, kom2):
    a = b = 1
    while a <= kom1:
        if a == kom1 and b == kom2:
            return True
        a, b = b, a+b
    return False

def zad117(T):
    dl = len(T)
    for i in range(dl-2):
        for j in range(dl-2):
            kom1 = T[i][j]
            kom2 = T[i+1][j+1]
            kom3 = T[i+2][j+2]
            if kom1 <= kom2 < kom3 and kom3 == kom1 + kom2:
                if czyfib(kom2, kom3):
                    index, ciag = 1, 3
                    while i + 2 + index < dl and j + 2 + index < dl:
                        kom1 = T[i+2+index][j+2+index]
                        if kom1 != kom2 + kom3:
                            return ciag
                        ciag += 1
                        index += 1
                        kom2, kom3 = kom3, kom1
                    else:
                        return ciag
                # end if
                return 3
    return 0    
                    
T = [
    [1, 2, 3, 4, 5],
    [2, 1, 5, 8, 13],
    [3, 5, 2, 13, 21],
    [4, 8, 13, 3, 34],
    [5, 13, 21, 34, 5]
]
print(zad117(T))  # 5