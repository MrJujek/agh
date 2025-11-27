use Northwind;

-- 1 a
select
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)),2) as 'Łączna wartość'
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
where
    exists (
select 1
from employees as sub
where sub.reportsto = e.employeeid
    )
group by e.employeeid, e.firstname, e.lastname
order by 3 desc

-- 1 b
select
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)),2) as 'Łączna wartość'
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
where
    not exists (
select 1
from employees as sub
where sub.reportsto = e.employeeid
    )
group by e.employeeid, e.firstname, e.lastname
order by 3 desc