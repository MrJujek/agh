CREATE OR REPLACE TRIGGER trg_check_availability
FOR INSERT OR UPDATE OF status, trip_id ON reservation
COMPOUND TRIGGER

    TYPE t_trip_ids IS TABLE OF trip.trip_id%TYPE INDEX BY PLS_INTEGER;
    v_affected_trips t_trip_ids;

    AFTER EACH ROW IS
    BEGIN
        IF :NEW.status IN ('N', 'P') THEN
            v_affected_trips(:NEW.trip_id) := :NEW.trip_id;
        END IF;
    END AFTER EACH ROW;

    AFTER STATEMENT IS
        v_taken_places INT;
        v_max_places INT;
        v_idx PLS_INTEGER;
    BEGIN
        v_idx := v_affected_trips.FIRST;
        WHILE v_idx IS NOT NULL LOOP
            
            SELECT max_no_places INTO v_max_places 
            FROM trip 
            WHERE trip_id = v_idx;

            SELECT COUNT(*) INTO v_taken_places
            FROM reservation
            WHERE trip_id = v_idx AND status IN ('N', 'P');

            IF v_taken_places > v_max_places THEN
                RAISE_APPLICATION_ERROR(-20020, 'No free places available for the trip with ID: ' || v_idx);
            END IF;

            v_idx := v_affected_trips.NEXT(v_idx);
        END LOOP;
    END AFTER STATEMENT;

END trg_check_availability;
/

CREATE OR REPLACE PROCEDURE p_add_reservation_5(
    p_trip_id IN INT,
    p_person_id IN INT
)
AS
    v_trip_date DATE;
BEGIN
    SELECT trip_date INTO v_trip_date
    FROM trip WHERE trip_id = p_trip_id;
    
    IF v_trip_date <= TRUNC(SYSDATE) THEN
        RAISE_APPLICATION_ERROR(-20001, 'The specified trip has already taken place or is taking place today.');
    END IF;

    -- trg_check_availability will automatically check availability and raise an error if there are no free places when we insert the reservation.
    -- trg_reservation_log will automatically handle availability and logging when we insert the reservation.
    INSERT INTO reservation (trip_id, person_id, status)
    VALUES (p_trip_id, p_person_id, 'N');

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20003, 'The specified trip does not exist.');
END;
/

CREATE OR REPLACE PROCEDURE p_modify_reservation_status_5(
    p_reservation_id IN INT,
    p_status IN CHAR
)
AS
    v_current_status CHAR(1);
BEGIN
    SELECT status INTO v_current_status
    FROM reservation WHERE reservation_id = p_reservation_id;

    IF v_current_status = p_status THEN
        RAISE_APPLICATION_ERROR(-20004, 'Rezerwacja ma już podany status.');
    END IF;

    -- trg_check_availability will automatically check availability and raise an error if there are no free places when we update the reservation.
    UPDATE reservation
    SET status = p_status
    WHERE reservation_id = p_reservation_id;

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR(-20006, 'The specified reservation does not exist.');
END;
/