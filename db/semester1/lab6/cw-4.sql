use Northwind;

-- 1 a
select
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)) / count(distinct e2.EmployeeID), 2) as 'Łączna wartość'
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
    join Employees as e2 on e.EmployeeID = e2.ReportsTo
group by e.employeeid, e.firstname, e.lastname
order by 3 desc

-- 1 b
select
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)), 2) as 'Łączna wartość'
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
    left join Employees as e2 on e.EmployeeID = e2.ReportsTo
where e2.ReportsTo is Null
group by e.employeeid, e.firstname, e.lastname
order by 3 desc