use library;

-- 1
select m.firstname, m.lastname, (select count(*)
    from juvenile j
    where j.adult_member_no = a.member_no) 'Liczba dzieci'
from adult a
    join member m on m.member_no = a.member_no

-- 2
select m.member_no, m.firstname, m.lastname, (select count(*)
    from juvenile j
    where j.adult_member_no = a.member_no) 'Liczba dzieci', (select count(*)
    from reservation r
    where r.member_no = a.member_no) 'Liczba zarezerwowanych',
    (select count(*)
    from loan l
    where l.member_no = a.member_no) 'Liczba wypożyczonych teraz',
    (select count(*)
    from loanhist lh
    where lh.member_no = a.member_no) 'Liczba wypożyczonych historycznie'
from adult a
    join member m on m.member_no = a.member_no
group by m.lastname, m.firstname, a.member_no, m.member_no;

-- 3
select m.member_no, m.firstname, m.lastname, (select count(*)
    from juvenile j
    where j.adult_member_no = a.member_no) 'Liczba dzieci',
    (select count(*)
    from reservation r
    where r.member_no = a.member_no) +
    (select count(*)
    from reservation r
        join juvenile j on j.member_no = r.member_no
    where j.adult_member_no = a.member_no) 'Liczba zarezerwowanych',
    (select count(*)
    from loan l
    where l.member_no = a.member_no) + (select count(*)
    from loan l
        join juvenile j on j.member_no = l.member_no
    where j.adult_member_no = a.member_no) 'Liczba wypożyczonych teraz'
from adult a
    join member m on m.member_no = a.member_no
group by m.lastname, m.firstname, a.member_no, m.member_no

-- 4
select title, (select count(*)
    from loanhist lh
    where t.title_no = lh.title_no and year(out_date) = 2001) as 'Liczba wypożyczeń'
from title t
order by 2 desc;

-- 5
select title, (select count(*)
    from loanhist lh
    where t.title_no = lh.title_no and year(out_date) = 2002) as 'Liczba wypożyczeń'
from title t
order by 2 desc;