use Northwind;

-- 1
select c.CompanyName, c.Address
from Customers c
where c.CustomerID not in (select o.CustomerID
from Orders o
where year(o.OrderDate) = 1997 and o.CustomerID = c.CustomerID)

-- 2
select c.CompanyName, c.Phone
from Customers c
where CustomerID in (select o.CustomerID
from Orders o join Shippers s on s.ShipperID = o.ShipVia
where year(o.OrderDate) = 1997 and o.CustomerID = c.CustomerID and s.CompanyName = 'United Package')

-- 3
select c.CompanyName, c.Phone
from Customers c
where CustomerID not in (select o.CustomerID
from Orders o join Shippers s on s.ShipperID = o.ShipVia
where year(o.OrderDate) = 1997 and o.CustomerID = c.CustomerID and s.CompanyName = 'United Package')