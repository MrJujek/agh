use Northwind;

-- 1
select ProductName, UnitPrice, CompanyName
from Products as p
    join Suppliers as s
    on p.SupplierID = s.SupplierID
where UnitPrice BETWEEN 20 and 30

-- 2
select ProductName, UnitsInStock
from Products as p
    join Suppliers as s
    on p.SupplierID = s.SupplierID
where CompanyName = 'Tokyo Traders'

-- 3
select distinct c.CustomerID, CompanyName, c.Address, year(OrderDate)
from Customers as c
    left join Orders as o
    on o.CustomerID = c.CustomerID and year(o.OrderDate) = 1997
where OrderDate is NULL

-- 4
select CompanyName, Phone
from Suppliers as s
    join Products as p
    on s.SupplierID = p.SupplierID
where UnitsInStock = 0

-- 5
select OrderID, OrderDate, CompanyName, Phone
from Orders as o
    join Customers as c
    on o.CustomerID = c.CustomerID
where MONTH(OrderDate) = 3 and YEAR(OrderDate) = 1997
