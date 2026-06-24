# Dokumentacja Projektu Bazy Danych

## Role Systemowe i Uprawnienia

W systemie zdefiniowano trzy główne role użytkowników, z których każda posiada specyficzny zestaw uprawnień dostosowany do pełnionych obowiązków. Uprawnienia są nadawane za pomocą skryptu `permissions.sql`.

### 1. Sprzedawca (`Role_Sales`)
Osoba odpowiedzialna za obsługę klienta i procesowanie zamówień.

*   **Dostęp do Procedur:**
    *   `sp_AddCustomer`: Dodawanie nowych klientów.
    *   `sp_CreateOrder`: Tworzenie nowych zamówień.
    *   `sp_AddOrderDetail`: Dodawanie pozycji do zamówienia.
    *   `sp_UpdateOrderStatus`: Aktualizacja statusu zamówienia.
    *   `sp_GetClientOrdersReport`: Generowanie raportów zamówień klienta.
*   **Dostęp do Danych:**
    *   Pełny dostęp (INSERT, UPDATE, SELECT) do tabel związanych ze sprzedażą: `Customers`, `Orders`, `OrderDetails`.
    *   Dostęp do odczytu (SELECT) listy produktów (`Products`) oraz statusów (`OrderStatus`).

### 2. Magazynier (`Role_Warehouse`)
Osoba zarządzająca stanem magazynowym i planem produkcji.

*   **Dostęp do Procedur:**
    *   `sp_RestockPart`: Uzupełnianie zapasów części w magazynie.
*   **Dostęp do Danych:**
    *   Pełny dostęp (INSERT, UPDATE, SELECT) do magazynu części (`PartsStorage`) i planu produkcji (`ProductionPlan`).
    *   Dostęp do odczytu (SELECT) produktów (`Products`) oraz ostrzeżeń o niskim stanie magazynowym (`StockWarnings`).

### 3. Kierownik (`Role_Manager`)
Osoba nadzorująca całość procesów, posiadająca najszersze uprawnienia.

*   **Dostęp do Procedur:**
    *   Możliwość wykonania **wszystkich** procedur w systemie.
*   **Dostęp do Danych:**
    *   Dostęp do odczytu (SELECT) **wszystkich** tabel w systemie.
    *   Specjalny dostęp do tabel audytowych, np. historii zmian cen (`ProductPriceHistory`).

## Uruchomienie Uprawnień
Aby wdrożyć powyższy model uprawnień, należy uruchomić skrypt:
```sql
source permissions.sql
```
Lub wykonać jego zawartość w narzędziu do zarządzania bazą danych (np. SSMS).
