ALTER TABLE trip ADD no_available_places INT NULL;

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
