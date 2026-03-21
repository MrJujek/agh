CREATE OR REPLACE PROCEDURE p_add_reservation(
    p_trip_id IN INT,
    p_person_id IN INT
)
AS
    v_trip_date DATE;
    v_taken_places INT;
    v_max_places INT;
    v_reservation_id INT;
BEGIN
    SELECT trip_date, max_no_places 
    INTO v_trip_date, v_max_places
    FROM trip 
    WHERE trip_id = p_trip_id;
    
    IF v_trip_date <= TRUNC(SYSDATE) THEN
        RAISE_APPLICATION_ERROR(-20001, 'Trip has already taken place or is taking place today.');
    END IF;

    SELECT COUNT(*) 
    INTO v_taken_places
    FROM reservation
    WHERE trip_id = p_trip_id AND status IN ('N', 'P');

    IF v_taken_places >= v_max_places THEN
        RAISE_APPLICATION_ERROR(-20002, 'No free places available for this trip.');
    END IF;

    INSERT INTO reservation (trip_id, person_id, status)
    VALUES (p_trip_id, p_person_id, 'N')
    RETURNING reservation_id INTO v_reservation_id;

    INSERT INTO log (reservation_id, log_date, status)
    VALUES (v_reservation_id, SYSDATE, 'N');

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20003, 'The specified trip does not exist.');
END;
/

CREATE OR REPLACE PROCEDURE p_modify_reservation_status(
    p_reservation_id IN INT,
    p_status IN CHAR
)
AS
    v_current_status CHAR(1);
    v_trip_id INT;
    v_taken_places INT;
    v_max_places INT;
BEGIN
    SELECT status, trip_id
    INTO v_current_status, v_trip_id
    FROM reservation
    WHERE reservation_id = p_reservation_id;

    IF v_current_status = p_status THEN
        RAISE_APPLICATION_ERROR(-20004, 'Reservation already has the specified status.');
    END IF;

    IF v_current_status = 'C' AND p_status IN ('N', 'P') THEN
        SELECT max_no_places INTO v_max_places FROM trip WHERE trip_id = v_trip_id;
        
        SELECT COUNT(*) INTO v_taken_places 
        FROM reservation 
        WHERE trip_id = v_trip_id AND status IN ('N', 'P');
        
        IF v_taken_places >= v_max_places THEN
            RAISE_APPLICATION_ERROR(-20005, 'No free places available. Cannot restore cancelled reservation.');
        END IF;
    END IF;

    UPDATE reservation
    SET status = p_status
    WHERE reservation_id = p_reservation_id;

    INSERT INTO log (reservation_id, log_date, status)
    VALUES (p_reservation_id, SYSDATE, p_status);

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20006, 'The specified reservation does not exist.');
END;
/

CREATE OR REPLACE PROCEDURE p_modify_max_no_places(
    p_trip_id IN INT,
    p_max_no_places IN INT
)
AS
    v_taken_places INT;
BEGIN
    SELECT COUNT(*) 
    INTO v_taken_places
    FROM reservation
    WHERE trip_id = p_trip_id AND status IN ('N', 'P');

    IF p_max_no_places < v_taken_places THEN
        RAISE_APPLICATION_ERROR(-20007, 'Cannot decrease the number of places below the number of active reservations (' || v_taken_places || ').');
    END IF;

    UPDATE trip
    SET max_no_places = p_max_no_places
    WHERE trip_id = p_trip_id;

    IF SQL%ROWCOUNT = 0 THEN
        RAISE_APPLICATION_ERROR(-20003, 'The specified trip does not exist.');
    END IF;
END;
/