use Northwind;

-- 1
select c.CategoryName, sum(od.Quantity) 'Zamówione'
from Categories as c
    join Products as p on c.CategoryID = p.CategoryID
    join [Order Details] as od on p.ProductID = od.ProductID
group by c.CategoryName

-- 2
select c.CategoryName, round(sum(od.Quantity * od.UnitPrice * (1 - od.Discount)), 2) 'Wartość'
from Categories as c
    join Products as p on c.CategoryID = p.CategoryID
    join [Order Details] as od on p.ProductID = od.ProductID
group by c.CategoryName

-- 3 a
select c.CategoryName, round(sum(od.Quantity * od.UnitPrice * (1 - od.Discount)), 2) 'Wartość'
from Categories as c
    join Products as p on c.CategoryID = p.CategoryID
    join [Order Details] as od on p.ProductID = od.ProductID
group by c.CategoryName
order by 2 desc

-- 3 b
select c.CategoryName, round(sum(od.Quantity * od.UnitPrice * (1 - od.Discount)), 2) 'Wartość'
from Categories as c
    join Products as p on c.CategoryID = p.CategoryID
    join [Order Details] as od on p.ProductID = od.ProductID
group by c.CategoryName
order by sum(od.Quantity) desc

-- 4
select c.CategoryName, round(sum(od.Quantity * od.UnitPrice * (1 - od.Discount) + o.Freight), 2) 'Wartość'
from Categories as c
    join Products as p on c.CategoryID = p.CategoryID
    join [Order Details] as od on p.ProductID = od.ProductID
    join Orders as o on od.OrderID = o.OrderID
group by c.CategoryName