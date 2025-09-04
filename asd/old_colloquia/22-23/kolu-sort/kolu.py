from kolutesty import runtests

def ice_cream(T):
    T.sort(reverse=True)

    total_eaten_volume = 0
    
    for i in range(len(T)):
        time_elapsed = i
        
        current_volume = T[i] - time_elapsed
        
        if current_volume > 0:
            total_eaten_volume += current_volume
        else:
            break
            
    return total_eaten_volume

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( ice_cream, all_tests = True )
