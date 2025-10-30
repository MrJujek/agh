use Northwind;

select ProductName, CompanyName
from Products
    INNER JOIN Suppliers
    ON Products.SupplierID = Suppliers.SupplierID;

select distinct companyname, orderdate
from orders
    INNER JOIN Customers
    on orders.customerid = customers.customerid
where OrderDate > '1998-03-01'

SELECT companyname, customers.customerid, orderdate
FROM customers
    LEFT OUTER JOIN orders
    ON customers.customerid = orders.customerid

SELECT companyname, customers.customerid, orderdate
FROM customers
    LEFT OUTER JOIN orders
    ON customers.customerid = orders.customerid
WHERE orderid is null