def merge_sort(tab):
    i = k = 0 
    j = len(tab) // 2
    ans = [0] * len(tab)
    while k < len(tab):
        if tab[i] < tab[j] and i < len(tab) // 2:
            ans[k] = tab[i]
            i += 1
        else:
            ans[k] = tab[j]
            j += 1
        k += 1

    return ans

tab = [1,2,4,8,11,3,5,7,9,10,13]
print(tab)
print(merge_sort(tab))