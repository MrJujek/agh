use Northwind;

select count(UnitPrice) from Products where UnitPrice between 10 and 20;

select max(UnitPrice) from Products where UnitPrice < 20;

select max(UnitPrice), min(UnitPrice), avg(UnitPrice) from Products where QuantityPerUnit like '%bottle%';

select * from Products where UnitPrice > (select avg(UnitPrice) from Products);

select round(sum(UnitPrice * Quantity * (1 - Discount)), 2) from [Order Details] where OrderID = 10250;