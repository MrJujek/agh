SET SERVEROUTPUT ON;

DECLARE
    v_trip_id_6a INT;
    v_trip_id_6b INT;
    v_person_id INT;
    v_person_id2 INT;
    
    v_res_id_6a INT;
    v_res_id_6b INT;

    v_avail INT;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Rozpoczynam testy dla zadań 6a i 6b...');
    DBMS_OUTPUT.PUT_LINE('-------------------------------------------');

    -- Pobieranie nowych ID dla danych testowych
    SELECT s_trip_seq.nextval INTO v_trip_id_6a FROM dual;
    SELECT s_trip_seq.nextval INTO v_trip_id_6b FROM dual;
    SELECT s_person_seq.nextval INTO v_person_id FROM dual;
    SELECT s_person_seq.nextval INTO v_person_id2 FROM dual;

    -- Dodajemy testowe wycieczki
    INSERT INTO trip (trip_id, trip_name, country, trip_date, max_no_places, no_available_places)
    VALUES (v_trip_id_6a, 'Test wycieczki 6A', 'Polska', SYSDATE + 10, 5, 5);

    INSERT INTO trip (trip_id, trip_name, country, trip_date, max_no_places, no_available_places)
    VALUES (v_trip_id_6b, 'Test wycieczki 6B', 'Polska', SYSDATE + 10, 5, 5);

    -- Dodajemy testowe osoby
    INSERT INTO person (person_id, firstname, lastname) 
    VALUES (v_person_id, 'TestUser1', 'Kowalski');
    
    INSERT INTO person (person_id, firstname, lastname) 
    VALUES (v_person_id2, 'TestUser2', 'Nowak');

    COMMIT;


    DBMS_OUTPUT.PUT_LINE('=== T E S T Y   6 A (Procedury) ===');
    
    -- WYŁĄCZAMY triggery z zadania 6B na czas testowania procedur z 6A
    EXECUTE IMMEDIATE 'ALTER TRIGGER trg_reservation_insert_6b DISABLE';
    EXECUTE IMMEDIATE 'ALTER TRIGGER trg_reservation_update_6b DISABLE';

    -- Test 1: Dodawanie rezerwacji
    p_add_reservation_6a(v_trip_id_6a, v_person_id);
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6a;
    IF v_avail = 4 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 1: p_add_reservation_6a zmniejsza wolne miejsca (5 -> 4).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 1: p_add_reservation_6a (Oczekiwano: 4, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Pobieramy ID właśnie dodanej rezerwacji
    SELECT reservation_id INTO v_res_id_6a 
    FROM reservation 
    WHERE trip_id = v_trip_id_6a AND person_id = v_person_id;

    -- Test 2: Anulowanie rezerwacji
    p_modify_reservation_status_6a(v_res_id_6a, 'C');
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6a;
    IF v_avail = 5 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 2: p_modify_reservation_status_6a N -> C zwiększa wolne miejsca (4 -> 5).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 2: p_modify_reservation_status_6a N -> C (Oczekiwano: 5, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Test 3: Przywracanie rezerwacji z anulowanej na potwierdzoną
    p_modify_reservation_status_6a(v_res_id_6a, 'P');
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6a;
    IF v_avail = 4 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 3: p_modify_reservation_status_6a C -> P zmniejsza wolne miejsca (5 -> 4).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 3: p_modify_reservation_status_6a C -> P (Oczekiwano: 4, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Test 4: Zmiana max liczby miejsc na wycieczce
    p_modify_max_no_places_6a(v_trip_id_6a, 10);
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6a;
    IF v_avail = 9 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 4: p_modify_max_no_places_6a uwzględnia aktywne rezerwacje po powiększeniu puli. (9)');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 4: p_modify_max_no_places_6a (Oczekiwano: 9, Otrzymano: ' || v_avail || ')');
    END IF;


    DBMS_OUTPUT.PUT_LINE(' ');
    DBMS_OUTPUT.PUT_LINE('=== T E S T Y   6 B (Triggery) ===');
    
    -- WŁĄCZAMY z powrotem triggery z zadania 6B na czas testów 6B
    EXECUTE IMMEDIATE 'ALTER TRIGGER trg_reservation_insert_6b ENABLE';
    EXECUTE IMMEDIATE 'ALTER TRIGGER trg_reservation_update_6b ENABLE';

    -- UWAGA: zakłada, że triggery z 6b są aktywne (trg_reservation_insert_6b, trg_reservation_update_6b)

    -- Test 5: Dodawanie rezerwacji z wykorzystaniem procedury 6b / triggerów 6b
    p_add_reservation_6b(v_trip_id_6b, v_person_id);
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6b;
    IF v_avail = 4 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 5: p_add_reservation_6b / trigger zmniejsza wolne miejsca (5 -> 4).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 5: p_add_reservation_6b / trigger (Oczekiwano: 4, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Pobieramy ID właśnie dodanej rezerwacji
    SELECT reservation_id INTO v_res_id_6b 
    FROM reservation 
    WHERE trip_id = v_trip_id_6b AND person_id = v_person_id;

    -- Test 6: Anulowanie rezerwacji
    p_modify_reservation_status_6b(v_res_id_6b, 'C');
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6b;
    IF v_avail = 5 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 6: p_modify_reservation_status_6b N -> C / trigger zwiększa wolne miejsca (4 -> 5).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 6: p_modify_reservation_status_6b N -> C (Oczekiwano: 5, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Test 7: Przywracanie rezerwacji z anulowanej na potwierdzoną
    p_modify_reservation_status_6b(v_res_id_6b, 'P');
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6b;
    IF v_avail = 4 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 7: p_modify_reservation_status_6b C -> P / trigger zmniejsza wolne miejsca (5 -> 4).');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 7: p_modify_reservation_status_6b C -> P (Oczekiwano: 4, Otrzymano: ' || v_avail || ')');
    END IF;

    -- Test 8: Zmiana max liczby miejsc na wycieczce (6b)
    p_modify_max_no_places_6b(v_trip_id_6b, 10);
    SELECT no_available_places INTO v_avail FROM trip WHERE trip_id = v_trip_id_6b;
    IF v_avail = 9 THEN
        DBMS_OUTPUT.PUT_LINE('[ OK ] Test 8: p_modify_max_no_places_6b uwzględnia aktywne rezerwacje. (9)');
    ELSE
        DBMS_OUTPUT.PUT_LINE('[FAIL] Test 8: p_modify_max_no_places_6b (Oczekiwano: 9, Otrzymano: ' || v_avail || ')');
    END IF;

    DBMS_OUTPUT.PUT_LINE('-------------------------------------------');
    DBMS_OUTPUT.PUT_LINE('Testy zakończone. Trwa sprzątanie danych posiecznych...');

    -- Sprzątanie (odwrócenie operacji w celu zachowania stanu bazy)
    DELETE FROM log WHERE reservation_id IN (v_res_id_6a, v_res_id_6b);
    DELETE FROM reservation WHERE reservation_id IN (v_res_id_6a, v_res_id_6b);
    DELETE FROM trip WHERE trip_id IN (v_trip_id_6a, v_trip_id_6b);
    DELETE FROM person WHERE person_id IN (v_person_id, v_person_id2);

    COMMIT;
    DBMS_OUTPUT.PUT_LINE('Sprzątanie zakończone pomyślnie. Czyste środowisko przywrócone.');

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Wystąpił błąd w trakcie testów: ' || SQLERRM);
        ROLLBACK;
END;
/
