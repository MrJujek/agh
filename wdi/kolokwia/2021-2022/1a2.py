def decimal_to_base4(dec):
    if dec == 0:
        return 0
    
    odp = 0
    multiplier = 1
    while dec > 0:
        odp += (dec % 4)*multiplier
        dec //= 4
        multiplier *= 10

    return odp

def check_4_correct(a, b):
    a_numbers = [False for _ in range(4)]
    b_numbers = [False for _ in range(4)]

    while a > 0:
        a_numbers[a % 10] = True
        a //= 10

    while b > 0:
        b_numbers[b % 10] = True
        b //= 10

    for i in range(4):
        if a_numbers[i] != b_numbers[i]:
            return False
        
    return True

def check_line(T):
    l = len(T)
    longest = 0
    for i in range(l):
        longest_pretender = 1
        for j in range(i+1, l):
            if check_4_correct(decimal_to_base4(T[i]), decimal_to_base4(T[j])):
                longest_pretender += 1
        longest = max(longest, longest_pretender)

    return longest

T = [13,23,13,33,107,57]
    
print(check_line(T))