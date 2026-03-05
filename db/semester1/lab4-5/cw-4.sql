use Northwind;

-- 1
select top 1
    s.CompanyName, count(*)
from Shippers as s
    inner join Orders as o
    on s.ShipperID = o.ShipVia
where year(ShippedDate) = 1997
group by CompanyName
order by 2 desc

-- 2
select o.OrderID, o.OrderDate, c.CompanyName, round(sum(od.UnitPrice * od.Quantity * (1 - od.Discount)), 2) as 'Wartość zamówionych produktów'
from Orders as o
    inner join [Order Details] as od
    on o.OrderID = od.OrderID
    inner join Customers as c
    on c.CustomerID = o.CustomerID
group by o.OrderID, o.OrderDate, c.CompanyName, c.CustomerID
order by 4 desc

-- 3
select o.OrderID, o.OrderDate, c.CompanyName, round(sum(od.UnitPrice * od.Quantity * (1 - od.Discount)) + o.Freight, 2) as 'Wartość zamówienia'
from Orders as o
    inner join [Order Details] as od
    on o.OrderID = od.OrderID
    inner join Customers as c
    on c.CustomerID = o.CustomerID
group by o.OrderID, o.OrderDate, c.CompanyName, c.CustomerID, o.Freight
order by 4 desc