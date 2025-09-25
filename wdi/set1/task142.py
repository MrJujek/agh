def waga(T, n, i=0, result=[]):
    if n == 0:
        print(result)
        return 
    if i == len(T):
        return
    waga(T, n-T[i], i+1, result + [T[i]])
    waga(T, n, i+1, result)
    waga(T, n+T[i], i+1, result + [ -T[i]])
    
waga([1,3,5,10,16,24], 23)