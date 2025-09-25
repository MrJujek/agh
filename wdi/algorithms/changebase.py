import math

def changebasefrom10(x, sys):
    chars = '0123456789ABCDEF'
    l = int(math.log(x, sys))+1
    odp = [0 for _ in range(l)]

    index = l - 1
    while x != 0:
        odp[index] = chars[x%sys]
        x//=sys
        index -= 1

    return odp
print(changebasefrom10(29,4))
