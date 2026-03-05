use Northwind;

--select CategoryID from Categories where CategoryName like 'Meat/Poultry';
--select ProductName, UnitPrice from Products where CategoryID = 6;

--select SupplierID from Suppliers where CompanyName like 'Tokyo Traders';
--select ProductName, UnitsInStock from Products where SupplierID = 4;

--select CompanyName from Customers where CompanyName like '%Restaurant%';

--select ProductID, ProductName, QuantityPerUnit from Products where QuantityPerUnit like '%bottle%';

--select LastName, Title from Employees where LastName like '[B-L]%';
--select LastName, Title from Employees where LastName like '[BL]%';

--select * from Products where UnitPrice not between 10 and 20;
--select ProductName, UnitPrice from Products where UnitPrice between 20 and 30;

--select * from Orders where year(OrderDate) = 1997;

--select OrderID, OrderDate, CustomerID from Orders where ShipCountry = 'Argentina' and (ShippedDate is NULL or getdate() < ShippedDate);

--select Country, CompanyName from Customers order by Country asc, CompanyName asc;

--select * from Customers where Country in ('France', 'Spain') order by country asc, CompanyName asc;

--select * from Orders where year(OrderDate) = 1997 ORDER BY MONTH(ShippedDate) DESC;

select UnitPrice, Quantity, Discount, (UnitPrice * Quantity * (1 - Discount)) as Cost from [Order Details] where OrderID = 10250;

select Phone + ', ' + Fax as 'Contact info' from Suppliers where Phone is not NULL and Fax is not NULL;
select isnull(Phone , '') + ', ' + isnull(Fax, '') as 'Contact info' from Suppliers;