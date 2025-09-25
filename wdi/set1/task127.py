def multi(T):
    maks = 0
    for napis in T:
        if wiel(napis):
            if len(napis) > maks:
                maks = len(napis)
    return maks

def wiel(napis):
    d = len(napis)
    flaga = True
    for baza in range(1, d // 2 + 1):
        if d % baza == 0:
            flaga = True
            for x in range(baza, d, baza):
                for z in range(baza):
                    if napis[z + x] != napis[z]:
                        flaga =  False
        # end if
        if flaga: return True
    # end for
    return False

print(multi(['ABCABCABC', 'AAAA', 'ABAABAABA']))