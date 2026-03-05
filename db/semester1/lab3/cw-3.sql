use Northwind;

select OrderID, count(*) from [Order Details] group by OrderID having count(OrderID) > 5;

select CustomerID, count(*) from Orders where year(ShippedDate) = 1998 group by CustomerID having count(CustomerID) > 8 order by sum(Freight) desc