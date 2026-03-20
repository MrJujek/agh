create or replace view vw_reservation as 
select
    r.reservation_id,
    t.country,
    t.trip_date,
    t.trip_name,
    p.firstname,
    p.lastname,
    r.status,
    t.trip_id,
    p.person_id
from reservation r
join trip t on r.trip_id = t.trip_id
join person p on r.person_id = p.person_id;

CREATE or replace view vw_reservation_count as
select
    t.trip_id,
    t.country,
    t.trip_date,
    t.trip_name,
    t.max_no_places,
    (t.max_no_places - count(r.reservation_id)) as no_available_places
from reservation r
join trip t on r.trip_id = t.trip_id
group by t.trip_id, t.country, t.trip_date, t.trip_name, t.max_no_places;

CREATE or replace view vw_available_trip as
select
    *
from vw_reservation_count
where no_available_places > 0 and trip_date > sysdate
order by trip_date;