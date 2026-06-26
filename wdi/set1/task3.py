for i in range(1, 1013):
    a = b = i
    ciag = []

    while a <= 2024:
        ciag.append(a) 
        a,b = b,a+b
    
    # print(ciag)
    
    if ciag[-1] == 2024:
        print(ciag)