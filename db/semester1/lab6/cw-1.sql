use Northwind;

-- 1
select od.OrderID, c.CompanyName, sum(od.Quantity) 'Liczba zamówionych jednostek towaru'
from Orders as o
    join [Order Details] as od on o.OrderID = od.OrderID
    join Customers as c on c.CustomerID = o.CustomerID
group by od.OrderID, c.CompanyName
order by 3 desc

-- 2
select od.OrderID, c.CompanyName, sum(od.Quantity) 'Liczba zamówionych jednostek towaru'
from Orders as o
    join [Order Details] as od on o.OrderID = od.OrderID
    join Customers as c on c.CustomerID = o.CustomerID
group by od.OrderID, c.CompanyName
having sum(od.Quantity) > 250
order by 3 desc

-- 3
select
    od.OrderID,
    c.CompanyName,
    round(sum(od.Quantity * (1 - od.Discount) * od.UnitPrice), 2) 'Wartość'
from Orders as o
    join [Order Details] as od on o.OrderID = od.OrderID
    join Customers as c on c.CustomerID = o.CustomerID
group by od.OrderID, c.CompanyName

-- 4
select
    od.OrderID,
    c.CompanyName,
    sum(od.Quantity) 'Liczba zamówionych jednostek towaru',
    round(sum(od.Quantity * (1 - od.Discount) * od.UnitPrice), 2) 'Wartość'
from Orders as o
    join [Order Details] as od on o.OrderID = od.OrderID
    join Customers as c on c.CustomerID = o.CustomerID
group by od.OrderID, c.CompanyName
having sum(od.Quantity) > 250

-- 5
select
    od.OrderID,
    c.CompanyName,
    e.LastName,
    e.FirstName,
    sum(od.Quantity) 'Liczba zamówionych jednostek towaru',
    round(sum(od.Quantity * (1 - od.Discount) * od.UnitPrice), 2) 'Wartość'
from Orders as o
    join [Order Details] as od on o.OrderID = od.OrderID
    join Customers as c on c.CustomerID = o.CustomerID
    join Employees as e on e.EmployeeID = o.EmployeeID
group by od.OrderID, c.CompanyName, e.LastName, e.FirstName
having sum(od.Quantity) > 250
