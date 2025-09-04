def bubblesort(T):
    for i in range(len(T) - 1):
        for j in range(i + 1, len(T)):
            if T[i] > T[j]:
                T[j], T[i] = T[i], T[j]
    return T

print(bubblesort([99,-1,-12,12,1,22,10,3,2,4,-1,0,0,0,0]))