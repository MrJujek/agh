-- 1 a
use Library;
select m.firstname, m.lastname, a.street + ', ' + a.city + ', ' + a.zip 'address'
from member m
    left join adult a on a.member_no = m.member_no
    left join juvenile j on j.member_no = a.member_no
    left join loan l on l.member_no = m.member_no
    left join loanhist lh on lh.member_no = m.member_no
where 'address' is not NULL

-- 1 b
use Library;
select m.firstname, m.lastname, (select count(*)
    from juvenile
    where adult_member_no = a.member_no), (select count(*)
    from loanhist
    where member_no = a.member_no)
from member m
    join adult a on a.member_no = m.member_no
    join loanhist lh on lh.member_no = m.member_no
group by m.member_no, m.firstname, m.lastname, a.member_no

-- 2
use Northwind;
select c.CompanyName, COUNT(*)
from Customers c
    join Orders o on o.CustomerID = c.CustomerID
    join [Order Details] od on od.OrderID = o.OrderID
    join Products p on p.ProductID = od.ProductID
where p.CategoryID = 3 and year(o.ShippedDate) = 1997
group by c.CompanyName, p.ProductName, c.CustomerID
HAVING count(*) >= 2


-- 3 a
use Northwind;
    select c.CompanyName, c.Phone
    from Customers c
EXCEPT
    select c.CompanyName, c.Phone
    from Customers c
        join Orders o on o.CustomerID = c.CustomerID
        join Shippers s on s.ShipperID = o.ShipVia
    where s.CompanyName = 'United Package' and
        year(o.OrderDate) = 1997


-- 3 b
use Northwind;
select c.CompanyName, c.Phone
from Customers c
where c.CustomerID not in (
select c.CustomerID
from Customers c
    join Orders o on o.CustomerID = c.CustomerID
    join Shippers s on s.ShipperID = o.ShipVia
where s.CompanyName = 'United Package'
    and year(o.OrderDate) = 1997
)