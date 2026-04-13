insert into person(firstname, lastname)
values ('Jakub', 'Nowak');
commit;

insert into person(firstname, lastname)
values ('Jakub', 'Kowalski');
ROLLBACK;