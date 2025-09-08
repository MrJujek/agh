# Kolokwium z Programowania Imperatywnego

## Część 1
Pytania testowe (15 pytań × 1 pkt = 15 pkt)

### 1
Które z poniższych stwierdzeń jest prawdziwe dla języka C?
- [ ] Każda funkcja musi zwracać wartość.
- [x] Zmienne globalne są domyślnie inicjalizowane zerem.
- [ ] `sizeof('a')` zwraca 1, ponieważ 'a' jest typu char.
- [ ] void* nie może być używany do przechowywania adresów funkcji.

### 2
Jaki będzie wynik wyrażenia 5 / 2 w języku C?
- [ ] 2.5
- [x] 2
- [ ] 3
- [ ] 2.0

### 3
Która z poniższych deklaracji jest poprawna w C?
- [x] `int arr[] = {1, 2, 3};`
- [ ] `int arr[3] = {1, 2, 3, 4};`
- [ ] `int arr[];`
- [x] `int arr[3] = {};`


### 4
Co zostanie wypisane po wykonaniu poniższego kodu?

```c
int x = 5;
printf("%d", x++ + ++x);
```
- [ ] 10
- [ ] 11
- [x] 12
- [ ] Zachowanie niezdefiniowane

### 5

Które z poniższych stwierdzeń dotyczących wskaźników w C jest fałszywe?
- [ ] Wskaźnik może wskazywać na inną zmienną wskaźnikową.
- [ ] Arytmetyka wskaźników jest dozwolona tylko w obrębie tej samej tablicy.
- [ ] void* może być rzutowany na dowolny typ wskaźnika bez utraty danych.
- [x] Wskaźnik na funkcję może być przypisany do wskaźnika na void*.

### 6
Która z poniższych instrukcji poprawnie alokuje pamięć dla tablicy 10 liczb całkowitych?
- [ ] `int* arr = malloc(10);`
- [x] `int* arr = malloc(10 * sizeof(int));`
- [ ] `int* arr = alloc(10, sizeof(int));`
- [ ] `int* arr = new int[10];`

### 7
Co zostanie wypisane po wykonaniu poniższego kodu?

```c
int x = 10;
int* p = &x;
int** pp = &p;
printf("%d", **pp);
```
- [x] 10
- [ ] Adres zmiennej x
- [ ] Adres zmiennej p
- [ ] Błąd kompilacji

### 8

Które z poniższych stwierdzeń dotyczących struktur w C jest prawdziwe?
- [ ] Struktury mogą zawierać funkcje jako składowe.
- [ ] Rozmiar struktury jest zawsze równy sumie rozmiarów jej pól.
- [x] Struktury mogą zawierać pola tego samego typu co sama struktura.
- [ ] Struktury domyślnie są przekazywane przez referencję do funkcji.

### 9
Która z poniższych funkcji jest poprawną implementacją zamiany wartości dwóch zmiennych?

- [ ] a 
```c
void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}
```

- [ ] b
```c
void swap(int* a, int* b) {
    int* temp = a;
    a = b;
    b = temp;
}
```

- [x] c
```c
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

- [ ] d
```c
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
```

### 10
Które z poniższych zdań dotyczących plików w C jest prawdziwe?
- [ ] fopen domyślnie otwiera plik w trybie binarnym.
- [ ] fseek może być używany do przesuwania wskaźnika pliku poza jego koniec.
- [ ] fread zawsze czyta dokładnie tyle bajtów, ile zostało określone.
- [x] feof zwraca true tylko po próbie odczytu po końcu pliku.

### 11
Co oznacza słowo kluczowe volatile w deklaracji zmiennej?
- [ ] Zmienna nie może być modyfikowana.
- [x] Zmienna może być modyfikowana przez inny wątek lub sprzęt.
- [ ] Zmienna jest przechowywana w rejestrze procesora.
- [ ] Zmienna jest automatycznie zerowana przy starcie programu.

### 12
Która z poniższych instrukcj`i poprawnie zwalnia pamięć zaalokowaną przez malloc?
- [x] `free(ptr);`
- [ ] `delete ptr;`
- [ ] `dealloc(ptr);`
- [ ] `ptr.free();`

### 13
Jaki będzie wynik wykonania poniższego kodu?
```c
int x = 5;
int y = x > 3 ? 10 : 20;
printf("%d", y);
```
- [ ] 5
- [x] 10
- [ ] 20
- [ ] Błąd kompilacji

### 14
Które z poniższych wyrażeń jest równoważne ```*(ptr + 3)```?
- [x] `ptr[3]`
- [ ] `3[ptr]`
- [ ] `*ptr + 3`
- [ ] `&ptr[3]`

### 15
Które z poniższych stwierdzeń dotyczących makr preprocesora jest fałszywe?
- [ ] Makra mogą przyjmować argumenty.
- [ ] Makra są rozwijane przed kompilacją.
- [ ] Makra mogą zawierać instrukcje warunkowe.
- [x] Makra są sprawdzane pod kątem typów danych.


## Część 2
Uzupełnianie kodu (5 zadań × 2 pkt = 10 pkt)

### 1
Uzupełnij poniższy kod, aby funkcja reverse odwracała napis w miejscu.

```c
void reverse(char* str) {
    int len = _______________;
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = _______________;
        _______________ = temp;
    }
}
```

### 2
Uzupełnij poniższy kod, aby funkcja count_ones zliczała liczbę bitów ustawionych na 1 w liczbie całkowitej.

```c
int count_ones(unsigned int num) {
    int count = 0;
    while (num) {
        count += _______________;
        num = _______________;
    }
    return count;
}
```

### 3
Uzupełnij poniższy kod, aby dynamicznie zaalokować tablicę struktur Student i zwolnić pamięć.

```c
typedef struct { char name[50]; int age; } Student;
int main() {
    Student* students = _______________;
    if (students == NULL) return 1;
    // ... użycie tablicy ...
    _______________;
    return 0;
}
```

### 4
Uzupełnij poniższy kod, aby otworzyć plik data.txt i wczytać z niego liczbę całkowitą.

```c
#include <stdio.h>
int main() {
    FILE* file = _______________;
    if (file == NULL) return 1;
    int num;
    _______________;
    fclose(file);
    printf("%d", num);
    return 0;
}
```

### 5
Uzupełnij poniższy kod, aby funkcja is_palindrome sprawdzała, czy napis jest palindromem.

```c
int is_palindrome(const char* str) {
    int left = 0;
    int right = _______________;
    while (left < right) {
        if (_______________) return 0;
        left++;
        right--;
    }
    return 1;
}
```


## Część 3
Analiza i rozwiązywanie problemów (5 zadań × 5 pkt = 25 pkt)

### 1
Co zostanie wypisane przez poniższy program? Wyjaśnij, dlaczego.

```c
#include <stdio.h>
int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int* p = arr + 2;
    printf("%d %d", p[-1], *(p + 1));
}
```
Output:
...................................................................<br>
Wyjaśnienie:
...................................................................

### 2
Znajdź błędy w poniższym kodzie i popraw je.

```c
#include <stdio.h>
int main() {
    int* ptr = malloc(sizeof(int));
    *ptr = 10;
    printf("%d", *ptr);
    free(ptr);
    printf("%d", *ptr);
    return 0;
}
```
Błędy:
...................................................................<br>
Poprawiony kod:
...................................................................

### 3
Jaka będzie zawartość tablicy arr po wykonaniu poniższego kodu?

```c
int arr[5] = {0};
for (int i = 0; i < 5; i++) {
    arr[i] = (i % 2) ? i * 2 : i + 1;
}
```
Zawartość arr:
...................................................................

### 4
Napisz funkcję, która łączy dwa napisy w jeden (konkatenacja) i zwraca nowy napis.

```c
char* concatenate(const char* str1, const char* str2) {
    // Uzupełnij kod
}
```

### 5
Wyjaśnij, dlaczego poniższy kod może prowadzić do wycieku pamięci.

```c
void process_data() {
    char* buffer = malloc(100);
    if (condition) return;
    free(buffer);
}
```