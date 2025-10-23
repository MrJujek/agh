use library;

-- select title_no, title from title;

-- select title from title where title_no = 10;

-- select title_no, author from title where author in ('Charles Dickens', 'Jane Austen');

-- select title_no, title from title where title like '%%adventure%';

-- select member_no, fine_paid from loanhist where month(out_date) = 11 and year(out_date) = 2001 and fine_paid is not NULL;

-- select distinct city, state from adult;

-- select title from title order by title asc;

-- select member_no, isbn, fine_assessed, fine_assessed * 2 as 'double fine' from loanhist where isnull(fine_assessed, 0) > 0;

-- select firstname + ' ' + middleinitial + ' ' + lastname as 'name' from member where lastname = 'Anderson';

-- select lower(firstname  + middleinitial + substring(lastname, 1, 2)) as 'name' from member where lastname = 'Anderson';

select concat('The title is: ', title, ', title number ', title_no) from title;
