select * from orders;
select * from customers;
select * from categories;
select * from Products;
select * from Shippers;

select * from orders;
select * from [Order Details]

select ReportsTo from Employees;

select Companyname, Address from Customers;
select LastName, HomePhone from Employees;
select ProductName, UnitPrice from Products;
select CategoryName, Description from Categories;
select CompanyName, Homepage from Suppliers;

select count(*) from Employees where country = 'USA';

select CompanyName, Address from customers where City = 'London'
select CompanyName, Address from customers where Country = 'France' or Country = 'Spain'
select ProductName, UnitPrice from Products where UnitPrice <= 30.00 and UnitPrice >= 20;