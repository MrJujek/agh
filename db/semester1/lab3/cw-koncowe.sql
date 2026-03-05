use Northwind;

-- cw 1
select top 10
    OrderID, sum(Freight) 'Wartość sprzedaży'
from Orders
group by OrderID
order by 'Wartość sprzedaży' DESC

-- cw 2
select OrderID, sum(Quantity) 'Liczba zamówionych'
from [Order Details]
group by OrderID

-- cw 3
select EmployeeID, count(EmployeeID) 'Obsługiwane zamówienia'
from Orders
group by EmployeeID

select ShipVia 'ShipperID', sum(Freight) 'Suma opłat'
from Orders
where year(ShippedDate) >= 1996 and year(ShippedDate) <= 1997
group by ShipVia

-- cw 4
select EmployeeID, month(OrderDate) 'Miesiąc', year(OrderDate) 'Rok', count(*) 'Liczba zamówień'
from Orders
group by EmployeeID, year(OrderDate), month(OrderDate)
order by 'Rok' asc

select CategoryID, min(UnitPrice) 'Cena minimalna', max(UnitPrice) 'Cena maksymalna'
from Products
group by CategoryID