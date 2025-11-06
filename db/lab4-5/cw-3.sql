use Northwind;

-- 1
select ProductName, UnitPrice, Address
from Products as p
    inner join Categories as c
    on p.CategoryID = c.CategoryID
    inner join Suppliers as s
    on p.SupplierID = s.SupplierID
where c.CategoryName = 'Meat/Poultry' and UnitPrice BETWEEN 20 and 30

-- 2
select ProductName, UnitPrice, CompanyName
from Products as p
    inner join Categories as c
    on c.CategoryID = p.CategoryID
    inner join Suppliers as s
    on p.SupplierID = s.SupplierID
where c.CategoryName = 'Confections'

-- 3
select c.CompanyName, count(*) as 'Liczba zamówień'
from Orders as o
    right join Customers as c
    on o.CustomerID = c.CustomerID
group by c.CompanyName, c.CustomerID
order by 2 desc

-- 4
select c.CompanyName, count(o.OrderID) as 'Liczba zamówień'
from Orders as o
    right join Customers as c
    on o.CustomerID = c.CustomerID
        and month(o.OrderDate) = 3 and year(o.OrderDate) = 1997
group by c.CustomerID, c.CompanyName
order by 2 desc