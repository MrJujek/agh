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

CREATE OR REPLACE FUNCTION f_available_trips_to(
    p_country IN VARCHAR2,
    p_date_from IN DATE,
    p_date_to IN DATE
)
RETURN SYS_REFCURSOR
AS
    v_cursor SYS_REFCURSOR;
BEGIN
    IF p_date_from > p_date_to THEN
        RAISE_APPLICATION_ERROR(-20003, 'Start date cannot be later than end date.');
    END IF;

    OPEN v_cursor FOR
    SELECT 
        t.trip_id,
        t.trip_name,
        t.country,
        t.trip_date,
        t.max_no_places,
        (t.max_no_places - (
            SELECT COUNT(*)
            FROM reservation r
            WHERE r.trip_id = t.trip_id AND r.status IN ('N', 'P')
        )) AS free_places
    FROM trip t
    WHERE t.country = p_country
      AND t.trip_date >= p_date_from
      AND t.trip_date <= p_date_to
      AND t.max_no_places > (
          SELECT COUNT(*)
          FROM reservation r
          WHERE r.trip_id = t.trip_id AND r.status IN ('N', 'P')
      );

    RETURN v_cursor;
END;
/