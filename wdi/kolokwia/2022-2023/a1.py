def sequence(T):
    l = len(T)
    for i in range(l - 2):
        for j in range(3, (l-i)//2 + 1):
            dobry_ciag = True

            tab1 = [T[z] for z in range(i, i + j)]
            tab2 = [T[z] for z in range(i+j, i + j + j)]
            
            mnoznik = tab2[0] / tab1[0]

            for z in range(1, len(tab1)):
                if tab2[z] / tab1[z] != mnoznik:
                    dobry_ciag = False
                    break
            
            if dobry_ciag:
                return [i, i+j-1]

tab = [2,5,7,3,2,3,5,7,6,9,15,21,17,19,23,2,6,4,8,3,5,7,1,3,2]
print(sequence(tab))