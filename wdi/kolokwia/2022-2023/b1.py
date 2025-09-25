def create_sequence(T, i):
    l = len(T)
    length = 1
    for j in range(i+1, l):
        if T[j-1] < T[j]:
            length +=1 
        else:
            break

    if length > 2:
        return (i, j - 1)
    else:
        return None
    
def sequence(T):
    l = len(T)
    potential_results = [None for _ in range(l)]
    current_index = 0
    for i in range(l):
        if i == 0 or T[i] < T[i-1]:
            result = create_sequence(T, i)
            if result is not None:
                potential_results[current_index] = result
                current_index += 1

    i = 0
    while potential_results[i] is not None:
        j = i + 1
        while potential_results[j] is not None:
            if T[potential_results[i][0]] > T[potential_results[j][1]] or T[potential_results[j][0]] > T[potential_results[i][1]]:
                return True
            j += 1
        i+=1

    return False

print(sequence( [2,15,17,13,17,19,23,2,6,4,8,3,5,7,1,3,2] )) # True
print(sequence( [2,15,17,13,17,19,23,2,6,4,8,3,5,7,14,3,2] )) # False