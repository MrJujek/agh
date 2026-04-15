# Oracle PL/Sql

widoki, funkcje, procedury, triggery

ćwiczenie 2

(kontynuacja ćwiczenia 1)

---

Imiona i nazwiska autorów: Julian Dworzycki, Radosław Klamka

---

<style>
  {
    font-size: 16pt;
  }
</style>

<style scoped>
 li, p {
    font-size: 14pt;
  }
</style>

<style scoped>
 pre {
    font-size: 10pt;
  }
</style>

# Zadanie 6

Zmiana struktury bazy danych. W tabeli `trip` należy dodać redundantne pole `no_available_places`. Dodanie redundantnego pola uprości kontrolę dostępnych miejsc (sprawdzenie liczby dostępnych miejsc), ale nieco skomplikuje procedury dodawania rezerwacji, zmiany statusu czy też zmiany maksymalnej liczby miejsc na wycieczki (potrzebna będzie dodatkowa aktualizacja w tabeli `trip`).

Należy przygotować polecenie/procedurę przeliczającą wartość pola `no_available_places` dla wszystkich wycieczek (do jednorazowego wykonania)

Obsługę pola `no_available_places` można zrealizować przy pomocy procedur lub triggerów

Należy zwrócić uwagę na spójność rozwiązania.

> UWAGA
> Należy stworzyć nowe wersje tych widoków/procedur/triggerów (np. dodając do nazwy dopisek 6 - od numeru zadania). Poprzednie wersje procedur należy pozostawić w celu umożliwienia weryfikacji ich poprawności.

- zmiana struktury tabeli

```sql
alter table trip add
    no_available_places int null
```

- polecenie przeliczające wartość `no_available_places`
  - należy wykonać operację "przeliczenia" liczby wolnych miejsc i aktualizacji pola `no_available_places`

# Zadanie 6 - rozwiązanie

```sql

UPDATE trip t
SET no_available_places = (
    t.max_no_places - (
        SELECT COUNT(*)
        FROM reservation r
        WHERE r.trip_id = t.trip_id
          AND r.status IN ('N', 'P')
    )
);

COMMIT;

SELECT
    t.trip_id,
    t.trip_name,
    t.max_no_places,
    t.no_available_places,
    (
        SELECT COUNT(*)
        FROM reservation r
        WHERE r.trip_id = t.trip_id
          AND r.status IN ('N', 'P')
    ) AS active_reservations
FROM trip t
ORDER BY t.trip_id;


```

---

# Zadanie 6a - procedury

Obsługę pola `no_available_places` należy zrealizować przy pomocy procedur

- procedura dodająca rezerwację powinna aktualizować pole `no_available_places` w tabeli trip
- podobnie procedury odpowiedzialne za zmianę statusu oraz zmianę maksymalnej liczby miejsc na wycieczkę
- należy przygotować procedury oraz jeśli jest to potrzebne, zaktualizować triggery oraz widoki

> UWAGA
> Należy stworzyć nowe wersje tych widoków/procedur/triggerów (np. dodając do nazwy dopisek 6a - od numeru zadania). Poprzednie wersje procedur należy pozostawić w celu umożliwienia weryfikacji ich poprawności.

- może być potrzebne wyłączenie 'poprzednich wersji' triggerów

# Zadanie 6a - rozwiązanie

```sql

ALTER TRIGGER trg_check_availability DISABLE;

CREATE OR REPLACE VIEW vw_available_trip_6a AS
SELECT
    trip_id,
    country,
    trip_date,
    trip_name,
    max_no_places,
    no_available_places
FROM trip
WHERE no_available_places > 0
  AND trip_date > SYSDATE
ORDER BY trip_date;

CREATE OR REPLACE PROCEDURE p_add_reservation_6a(
    p_trip_id   IN INT,
    p_person_id IN INT
)
AS
    v_trip_count       INT;
    v_person_count     INT;
    v_available_places INT;
    v_trip_date        DATE;
    v_already_reserved INT;
BEGIN
    SELECT COUNT(*) INTO v_trip_count
    FROM trip WHERE trip_id = p_trip_id;

    IF v_trip_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Trip with the given ID does not exist.');
    END IF;

    SELECT COUNT(*) INTO v_person_count
    FROM person WHERE person_id = p_person_id;

    IF v_person_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20002, 'Person with the given ID does not exist.');
    END IF;

    SELECT trip_date, no_available_places
    INTO v_trip_date, v_available_places
    FROM trip WHERE trip_id = p_trip_id;

    IF v_trip_date <= SYSDATE THEN
        RAISE_APPLICATION_ERROR(-20003, 'The trip has already taken place.');
    END IF;

    IF v_available_places <= 0 THEN
        RAISE_APPLICATION_ERROR(-20004, 'No available places for this trip.');
    END IF;

    SELECT COUNT(*) INTO v_already_reserved
    FROM reservation
    WHERE trip_id = p_trip_id
      AND person_id = p_person_id
      AND status IN ('N', 'P');

    IF v_already_reserved > 0 THEN
        RAISE_APPLICATION_ERROR(-20005, 'The person already has an active reservation for this trip.');
    END IF;

    INSERT INTO reservation(trip_id, person_id, status)
    VALUES (p_trip_id, p_person_id, 'N');

    UPDATE trip
    SET no_available_places = no_available_places - 1
    WHERE trip_id = p_trip_id;

    INSERT INTO log(reservation_id, log_date, status)
    VALUES (s_reservation_seq.currval, SYSDATE, 'N');

    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

CREATE OR REPLACE PROCEDURE p_modify_reservation_status_6a(
    p_reservation_id IN INT,
    p_new_status     IN CHAR
)
AS
    v_current_status   CHAR(1);
    v_trip_id          INT;
    v_available_places INT;
    v_trip_date        DATE;
BEGIN
    SELECT r.status, r.trip_id, t.trip_date, t.no_available_places
    INTO v_current_status, v_trip_id, v_trip_date, v_available_places
    FROM reservation r
    JOIN trip t ON r.trip_id = t.trip_id
    WHERE r.reservation_id = p_reservation_id;

    IF p_new_status NOT IN ('N', 'P', 'C') THEN
        RAISE_APPLICATION_ERROR(-20010, 'Invalid status. Allowed values: N, P, C.');
    END IF;

    IF v_current_status = p_new_status THEN
        RAISE_APPLICATION_ERROR(-20011, 'The reservation already has the given status.');
    END IF;

    IF v_current_status = 'C' AND p_new_status IN ('N', 'P') THEN
        IF v_trip_date <= SYSDATE THEN
            RAISE_APPLICATION_ERROR(-20012, 'Cannot restore reservation - the trip has already taken place.');
        END IF;
        IF v_available_places <= 0 THEN
            RAISE_APPLICATION_ERROR(-20013, 'Cannot restore reservation - no available places.');
        END IF;
    END IF;

    UPDATE reservation
    SET status = p_new_status
    WHERE reservation_id = p_reservation_id;

    -- Update no_available_places depending on the direction of status change:
    --   C -> N or C -> P : active reservation => takes a place
    --   N -> C or P -> C : cancelled reservation => frees a place
    --   N -> P or P -> N : change between active statuses => no impact on places
    IF v_current_status = 'C' AND p_new_status IN ('N', 'P') THEN
        UPDATE trip
        SET no_available_places = no_available_places - 1
        WHERE trip_id = v_trip_id;
    ELSIF v_current_status IN ('N', 'P') AND p_new_status = 'C' THEN
        UPDATE trip
        SET no_available_places = no_available_places + 1
        WHERE trip_id = v_trip_id;
    END IF;

    INSERT INTO log(reservation_id, log_date, status)
    VALUES (p_reservation_id, SYSDATE, p_new_status);

    COMMIT;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20009, 'Reservation with the given ID does not exist.');
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

CREATE OR REPLACE PROCEDURE p_modify_max_no_places_6a(
    p_trip_id       IN INT,
    p_max_no_places IN INT
)
AS
    v_active_reservations INT;
    v_trip_count          INT;
BEGIN
    SELECT COUNT(*) INTO v_trip_count
    FROM trip WHERE trip_id = p_trip_id;

    IF v_trip_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20019, 'Trip with the given ID does not exist.');
    END IF;

    SELECT COUNT(*) INTO v_active_reservations
    FROM reservation
    WHERE trip_id = p_trip_id
      AND status IN ('N', 'P');

    IF p_max_no_places < v_active_reservations THEN
        RAISE_APPLICATION_ERROR(-20020,
            'New number of places (' || p_max_no_places ||
            ') cannot be smaller than the number of active reservations (' ||
            v_active_reservations || ').');
    END IF;

    UPDATE trip
    SET max_no_places       = p_max_no_places,
        no_available_places = p_max_no_places - v_active_reservations
    WHERE trip_id = p_trip_id;

    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

```

---

# Zadanie 6b - triggery

Obsługę pola `no_available_places` należy zrealizować przy pomocy triggerów

- podczas dodawania rezerwacji trigger powinien aktualizować pole `no_available_places` w tabeli trip
- podobnie, podczas zmiany statusu rezerwacji
- należy przygotować trigger/triggery oraz jeśli jest to potrzebne, zaktualizować procedury modyfikujące dane oraz widoki

> UWAGA
> Należy stworzyć nowe wersje tych widoków/procedur/triggerów (np. dodając do nazwy dopisek 6b - od numeru zadania). Poprzednie wersje procedur należy pozostawić w celu umożliwienia weryfikacji ich poprawności.

- może być potrzebne wyłączenie 'poprzednich wersji' triggerów

# Zadanie 6b - rozwiązanie

```sql
CREATE OR REPLACE TRIGGER trg_reservation_insert_6b
BEFORE INSERT ON reservation
FOR EACH ROW
DECLARE
    v_available_places INT;
    v_trip_date        DATE;
BEGIN
    IF :NEW.status IN ('N', 'P') THEN
        SELECT trip_date, no_available_places
        INTO v_trip_date, v_available_places
        FROM trip
        WHERE trip_id = :NEW.trip_id;

        IF v_trip_date <= SYSDATE THEN
            RAISE_APPLICATION_ERROR(-20030, 'The trip has already taken place - cannot add reservation.');
        END IF;

        IF v_available_places <= 0 THEN
            RAISE_APPLICATION_ERROR(-20031, 'No available places for this trip.');
        END IF;

        UPDATE trip
        SET no_available_places = no_available_places - 1
        WHERE trip_id = :NEW.trip_id;
    END IF;
END;
/

CREATE OR REPLACE TRIGGER trg_reservation_update_6b
BEFORE UPDATE OF status ON reservation
FOR EACH ROW
DECLARE
    v_available_places INT;
    v_trip_date        DATE;
BEGIN
    SELECT trip_date, no_available_places
    INTO v_trip_date, v_available_places
    FROM trip
    WHERE trip_id = :NEW.trip_id;

    -- C -> N or C -> P: reactivation of reservation - needs an available place
    IF :OLD.status = 'C' AND :NEW.status IN ('N', 'P') THEN
        IF v_trip_date <= SYSDATE THEN
            RAISE_APPLICATION_ERROR(-20032, 'The trip has already taken place - cannot restore reservation.');
        END IF;
        IF v_available_places <= 0 THEN
            RAISE_APPLICATION_ERROR(-20033, 'No available places - cannot restore reservation.');
        END IF;

        UPDATE trip
        SET no_available_places = no_available_places - 1
        WHERE trip_id = :NEW.trip_id;

    -- N -> C or P -> C: reservation cancellation - frees a place
    ELSIF :OLD.status IN ('N', 'P') AND :NEW.status = 'C' THEN
        UPDATE trip
        SET no_available_places = no_available_places + 1
        WHERE trip_id = :NEW.trip_id;

    -- N -> P or P -> N: change between active statuses - no impact on places
    END IF;
END;
/

CREATE OR REPLACE PROCEDURE p_add_reservation_6b(
    p_trip_id   IN INT,
    p_person_id IN INT
)
AS
    v_trip_count       INT;
    v_person_count     INT;
    v_already_reserved INT;
BEGIN
    SELECT COUNT(*) INTO v_trip_count
    FROM trip WHERE trip_id = p_trip_id;

    IF v_trip_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Trip with the given ID does not exist.');
    END IF;

    SELECT COUNT(*) INTO v_person_count
    FROM person WHERE person_id = p_person_id;

    IF v_person_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20002, 'Person with the given ID does not exist.');
    END IF;

    SELECT COUNT(*) INTO v_already_reserved
    FROM reservation
    WHERE trip_id = p_trip_id
      AND person_id = p_person_id
      AND status IN ('N', 'P');

    IF v_already_reserved > 0 THEN
        RAISE_APPLICATION_ERROR(-20005, 'The person already has an active reservation for this trip.');
    END IF;

    -- Trigger trg_reservation_insert_6b handles availability control and field update
    INSERT INTO reservation(trip_id, person_id, status)
    VALUES (p_trip_id, p_person_id, 'N');

    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

CREATE OR REPLACE PROCEDURE p_modify_reservation_status_6b(
    p_reservation_id IN INT,
    p_new_status     IN CHAR
)
AS
    v_current_status CHAR(1);
BEGIN
    SELECT status INTO v_current_status
    FROM reservation WHERE reservation_id = p_reservation_id;

    IF p_new_status NOT IN ('N', 'P', 'C') THEN
        RAISE_APPLICATION_ERROR(-20010, 'Invalid status. Allowed values: N, P, C.');
    END IF;

    IF v_current_status = p_new_status THEN
        RAISE_APPLICATION_ERROR(-20011, 'The reservation already has the given status.');
    END IF;

    -- Trigger trg_reservation_update_6b handles availability control and field update
    UPDATE reservation
    SET status = p_new_status
    WHERE reservation_id = p_reservation_id;

    INSERT INTO log(reservation_id, log_date, status)
    VALUES (p_reservation_id, SYSDATE, p_new_status);

    COMMIT;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20009, 'Reservation with the given ID does not exist.');
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

CREATE OR REPLACE PROCEDURE p_modify_max_no_places_6b(
    p_trip_id       IN INT,
    p_max_no_places IN INT
)
AS
    v_active_reservations INT;
    v_trip_count          INT;
BEGIN
    SELECT COUNT(*) INTO v_trip_count
    FROM trip WHERE trip_id = p_trip_id;

    IF v_trip_count = 0 THEN
        RAISE_APPLICATION_ERROR(-20019, 'Trip with the given ID does not exist.');
    END IF;

    SELECT COUNT(*) INTO v_active_reservations
    FROM reservation
    WHERE trip_id = p_trip_id
      AND status IN ('N', 'P');

    IF p_max_no_places < v_active_reservations THEN
        RAISE_APPLICATION_ERROR(-20020,
            'New number of places (' || p_max_no_places ||
            ') cannot be smaller than the number of active reservations (' ||
            v_active_reservations || ').');
    END IF;

    UPDATE trip
    SET max_no_places       = p_max_no_places,
        no_available_places = p_max_no_places - v_active_reservations
    WHERE trip_id = p_trip_id;

    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
/

```
