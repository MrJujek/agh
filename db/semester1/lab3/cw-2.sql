use Northwind;

select OrderID, max(UnitPrice) as 'maxi', max(UnitPrice * Quantity * (1 - Discount)) from [Order Details] group by OrderID order by 'maxi' desc

select OrderID, min(UnitPrice) as 'mini', max(UnitPrice) as 'maxi' from [Order Details] group by OrderID;

select ShipVia, count(OrderID) from Orders group by ShipVia

select top 1 count(OrderID) as 'number' from Orders where year(ShippedDate) = 1997 group by ShipVia order by 'number' desc