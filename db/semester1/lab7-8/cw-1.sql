use Northwind;

-- 1
select OrderID, ROUND(SUM(UnitPrice * Quantity * (1 - Discount)) + (
    select Freight
    from Orders
    where OrderID = 10250
    ), 2) "Łączna wartość"
from [Order Details]
where OrderID = 10250
GROUP BY OrderID

-- 2
select OrderID, ROUND(SUM(UnitPrice * Quantity * (1 - Discount)) + (
    select Freight
    from Orders
    where OrderID = od.OrderID
    ), 2) "Łączna wartość"
from [Order Details] as od
GROUP BY OrderID

-- 3
select p.ProductName, (select max(od.UnitPrice * od.Quantity * (1 - od.Discount))
    from [Order Details] as od
    where p.ProductID = od.ProductID) "Maksymalna wartość"
from Products as p
order by 2 desc

-- 4
select p.ProductName, (select max(od.UnitPrice * od.Quantity * (1 - od.Discount))
    from [Order Details] as od
        join Orders as o on o.OrderID = od.OrderID and year(o.OrderDate) = 1997
    where p.ProductID = od.ProductID) "Maksymalna wartość w 1997"
from Products as p
order by 2 desc