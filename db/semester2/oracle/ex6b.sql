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