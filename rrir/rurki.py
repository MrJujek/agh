import sys
import numpy as np
import matplotlib.pyplot as plt

PHI_START = 5.0
PHI_END = 2.0

L_START = 0.0
L_END = 3.0

CONST_G = 1.0

def rho(x):
    if 1 < x <= 2:
        return 1.0
    else:
        return -10.0

def solve_fem(n):
    h = (L_END - L_START) / n
    
    nodes = np.linspace(L_START, L_END, n + 1)

    # Macierz sztywności i wektor obciążeń
    A = np.zeros((n + 1, n + 1))
    b = np.zeros(n + 1)
    
    # Punkty i wagi do całkowania Gaussa (2-punktowe)
    xi_gauss = [-1/np.sqrt(3), 1/np.sqrt(3)]
    w_gauss = [1.0, 1.0]
    
    # Stała w całkowaniu
    FACTOR = -4.0 * np.pi * CONST_G 

    print(f"Rozpoczynam obliczenia dla n={n} elementów...")

    for element in range(n):
        i = element
        j = element + 1
        x_i = nodes[i]
        x_j = nodes[j]
        
        # Macierz lokalna dla elementu liniowego (e_i)
        k_local = (1.0 / h) * np.array([[1, -1], [-1, 1]])
        
        # Agregacja do macierzy globalnej
        A[i, i] += k_local[0, 0]
        A[i, j] += k_local[0, 1]
        A[j, i] += k_local[1, 0]
        A[j, j] += k_local[1, 1]
        
        # Wektor obciążeń (całkowanie numeryczne)
        J = h / 2.0
        for k in range(2):
            xi = xi_gauss[k]
            w = w_gauss[k]
            x_real = (x_j + x_i) / 2.0 + (x_j - x_i) / 2.0 * xi
            
            N1 = (1 - xi) / 2.0
            N2 = (1 + xi) / 2.0
            
            f_val = FACTOR * rho(x_real)
            
            b[i] += w * f_val * N1 * J
            b[j] += w * f_val * N2 * J
    
    # Lewy brzeg (x = 0)
    A[0, :] = 0.0       
    A[0, 0] = 1.0       
    b[0] = PHI_START
    
    # Prawy brzeg (x = 3)
    A[n, :] = 0.0       
    A[n, n] = 1.0       
    b[n] = PHI_END

    try:
        u = np.linalg.solve(A, b)
    except np.linalg.LinAlgError:
        print("Błąd: Macierz osobliwa.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(nodes, u, marker='o', markersize=4, linestyle='-', label=f'MES (n={n})')
    plt.title(f'Problem 4.4: Potencjał grawitacyjny\nLiczba elementów n={n}')
    plt.xlabel('x')
    plt.ylabel('Phi(x)')
    plt.grid(True, which='both', linestyle='--')
    plt.legend()
    
    plt.axvspan(0, 1, color='red', alpha=0.1, label='rho = -10')
    plt.axvspan(1, 2, color='green', alpha=0.1, label='rho = 1')
    plt.axvspan(2, 3, color='blue', alpha=0.1, label='rho = -10')
    
    filename = f"rozwiazanie_n_{n}.png"
    plt.savefig(filename)
    print(f"Wykres zapisano do pliku: {filename}")
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        try:
            n_input = int(sys.argv[1])
            if n_input < 1:
                raise ValueError
            solve_fem(n_input)
        except ValueError:
            print("Błąd: Parametr n musi być liczbą całkowitą > 0.")
    else:
        print("Użycie: python zadanie.py <n>")