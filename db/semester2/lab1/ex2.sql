create or replace FUNCTION f_trip_participants(p_trip_id IN INT)
RETURN SYS_REFCURSOR
AS
    v_cursor SYS_REFCURSOR;
BEGIN
    DECLARE
        v_count INT;
    BEGIN
        SELECT COUNT(*) INTO v_count
        FROM trip t
        WHERE t.trip_id = p_trip_id;
        IF v_count = 0 THEN
            RAISE_APPLICATION_ERROR(-20001, 'Trip with given ID does not exist.');
        END IF;
    END;
    OPEN v_cursor FOR
    SELECT
        r.reservation_id,
        t.country,
        t.trip_date,
        t.trip_name,
        p.firstname,
        p.lastname,
        r.status,
        t.trip_id,
        p.person_id
    FROM reservation r
    JOIN trip t ON r.trip_id = t.trip_id
    JOIN person p ON r.person_id = p.person_id
    WHERE r.trip_id = p_trip_id;

    RETURN v_cursor;
END;
/

create or replace function f_person_reservations(p_person_id IN INT)
RETURN SYS_REFCURSOR
AS
    v_cursor SYS_REFCURSOR;

BEGIN
    DECLARE

        v_count INT;
    BEGIN
        SELECT COUNT(*) INTO v_count
        FROM person p
        WHERE p.person_id = p_person_id;
        IF v_count = 0 THEN
            RAISE_APPLICATION_ERROR(-20002, 'Person with given ID does not exist.');
        END IF;
    END;
    OPEN v_cursor FOR
    SELECT

        r.reservation_id,
        t.country,
        t.trip_date,
        t.trip_name,
        p.firstname,
        p.lastname,
        r.status,
        t.trip_id,
        p.person_id
    FROM reservation r
    JOIN trip t ON r.trip_id = t.trip_id
    JOIN person p ON r.person_id = p.person_id
    WHERE r.person_id = p_person_id;

    RETURN v_cursor;
END;
/