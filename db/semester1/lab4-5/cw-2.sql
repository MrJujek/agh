use Library;

-- 1
select firstname, lastname, birth_date
from member as m
    right join juvenile as j
    on m.member_no = j.member_no

-- 2
select distinct title
from title as t
    join loan as l
    on l.title_no = t.title_no

-- 3
select in_date, due_date, DATEDIFF(day, due_date, in_date) as 'Po czasie w dniach', ISNULL(fine_assessed, 0)
from title as t
    join loanhist as l
    on l.title_no = t.title_no
where title = 'Tao Teh King' and l.in_date > l.due_date

-- 4
select isbn, firstname, middleinitial, lastname
from reservation as r
    join member as m
    on m.member_no = r.member_no
where lastname = 'Graff' and firstname = 'Stephen' and middleinitial = 'A'
