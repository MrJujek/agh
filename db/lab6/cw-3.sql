use Northwind;

-- 1
select s.CompanyName, count(o.OrderID)
from Shippers as s
    join Orders as o on o.ShipVia = s.ShipperID and year(o.ShippedDate) = 1997
group by s.CompanyName

-- 2
select top 1
    s.CompanyName,
    count(o.OrderID) as 'Liczba zamówień'
from Shippers as s
    join Orders as o on s.ShipperID = o.ShipVia
where year(o.OrderDate) = 1997
group by s.CompanyName
order by 2 desc

-- 3
select
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)), 2) as 'Łączna wartość'
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
group by e.firstname, e.lastname
order by 3 desc

-- 4
select top 1
    e.firstname,
    e.lastname,
    count(o.orderid) as 'Liczba zamówień'
from employees as e
    join orders as o on e.employeeid = o.employeeid
where year(o.orderdate) = 1997
group by e.firstname, e.lastname
order by 3 desc

-- 5
select top 1
    e.firstname,
    e.lastname,
    round(sum(od.unitprice * od.quantity * (1 - od.discount)),2) as lacznawartosc
from employees as e
    join orders as o on e.employeeid = o.employeeid
    join [order details] as od on o.orderid = od.orderid
where year(o.orderdate) = 1997
group by e.firstname, e.lastname
order by 3 desc