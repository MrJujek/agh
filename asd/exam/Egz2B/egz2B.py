# Julian Dworzycki
# Złożoność obliczeniowa: O(n^2)
# Tworzę tablicę ends w której będę przechowywał dobre końce odcinków.
# Dla każdego elementu w T sprawdzam czy odcinek będzie kolidował z odcinkami w tablicy ends.
# Sprawdzam to na kopii tablicy, gdyż jeżeli jest kolizja i usunę element z tablicy w trakcie bycia w bętli for to ten for nie ogarnia :c
# Jeżeli była kolizja to usuwam wszytskie kolidujące końce z ends i nie dodaję nowego końca.
# Jak nie było kolizji to dodaję koniec do ends.
# Na koniec zwracam długość tablicy ends czyli liczbę dobrych odcinków. 

from egz2Btesty import runtests

def bitgame(T):
    ends = []
    
    for v in T:
        colision = False
        
        cp = []
        for u in ends:
            cp.append(u)

        for u in cp:
            if u <= v:
                ends.remove(u)
                colision = True

        if not colision:
          ends.append(v)
        
    return len(ends)

T = [5, 4, 3, 2, 4, 3, 1]
# print(bitgame(T))

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( bitgame, all_tests = True )
