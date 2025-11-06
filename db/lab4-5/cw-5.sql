use Northwind;

-- 1
select distinct cu.CompanyName, cu.Phone
from Products as p
    inner join Categories as c
    on p.CategoryID = c.CategoryID
        and c.CategoryName = 'Confections'
    inner join [Order Details] as od
    on od.ProductID = p.ProductID
    inner join Orders as o
    on o.OrderID = od.OrderID
    inner join Customers as cu
    on cu.CustomerId = o.CustomerID

-- 2
select c.CompanyName, c.Phone
from Customers as c
    left join Orders as o
    on c.CustomerID = o.CustomerID
    left join [Order Details] as od
    on od.OrderID = o.OrderID
    left join Products as p on od.ProductID = p.ProductID
    left join Categories as cat
    on p.CategoryID = cat.CategoryID
        and cat.CategoryName = 'Confections'
group by c.CustomerID, c.CompanyName, c.Phone
having count(cat.CategoryName) = 0
order by count(cat.CategoryName) desc

-- 3
select c.CompanyName, c.Phone
from Customers as c
    left join Orders as o
    on c.CustomerID = o.CustomerID
    left join [Order Details] as od
    on od.OrderID = o.OrderID
    left join Products as p on od.ProductID = p.ProductID
    left join Categories as cat
    on p.CategoryID = cat.CategoryID
        and cat.CategoryName = 'Confections'
        and year(o.OrderDate) = 1997 
group by c.CustomerID, c.CompanyName, c.Phone
having count(cat.CategoryName) = 0 and count(o.OrderDate) = 0
order by count(cat.CategoryName) desc