#   Złożoność czasowa: O(N + n log n) (porządkowanie leksykograficzne słów + sortowanie tablicy)
#   Złośoność pamięciowa: O(n^2) (pesymistycznie bo quicksort)

#   Zamieniam słowa w tablicy, tak aby ułożone leksykograficznie (def is_ordered).
#   Sortuję słowa, tak aby były ułożone leksykograficznie w tablicy (def quicksort).
#   Mając ułożone słowa przechodzę po nich i sprawdzam czy nowe słowo jest inne od poprzedniego
#   w ten sposób dostaję liczbę oznaczającą najdłuższy ciąg tego samego słowa, który oznacza siłę danego słowa.
#   Jeżeli słowo się zmieni a pozostałych elementów w tablicy jest mniej niż obecna największa siła,
#   to wiem, że nie znajdę już słowa o większej sile, więc ją zwracam.

from zad1testy import runtests

def strong_string(T):
    def is_ordered(word):
        l = 0
        r = len(word) - 1

        while l < r:
            if word[l] > word[r]:
                return False
            elif word[l] < word[r]:
                return True

            l += 1
            r -= 1

        return True

    def quicksort(T):
        l = len(T)
        if l <= 1:
            return T
        
        pivot = T[l//2]
        left = []
        middle = []
        right = []

        for i in T:
            if i < pivot:
                left.append(i)
            elif i == pivot:
                middle.append(i)
            else:
                right.append(i)
        
        return quicksort(left) + middle + quicksort(right)

    l = len(T)

    for i in range(l):
        if not is_ordered(T[i]):
            T[i] = T[i][::-1]

    T = quicksort(T)

    top = -1
    word = T[0]
    curr = 1
    for i in range(1, l):
        if T[i] != word:
            top = max(top, curr)

            if top >= l - i:
                return top

            curr = 1
            word = T[i]
        else:
            curr += 1
            word = T[i]

    return top

# strong_string(['kot', 'tok', 'kot', 'pies', 'mysz', 'seip', 'kogut'])


# Odkomentuj by uruchomic duze testy
runtests( strong_string, all_tests=True )

# Zakomentuj gdy uruchamiasz duze testy
# runtests( strong_string, all_tests=False )
