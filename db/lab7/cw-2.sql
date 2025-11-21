use Northwind;

-- 1
select
    c.CustomerID,
    c.CompanyName,
    (
    select
        round(sum(od.UnitPrice * od.Quantity * (1 - od.Discount)), 2)
    from
        Orders as o
        join [Order Details] as od on o.OrderID = od.OrderID
    where
      o.CustomerID = c.CustomerID
        and year(o.OrderDate) = 1996
  ) as "Wartość zamowień w 1996"
from
    Customers as c;

-- 2
select
    c.CustomerID,
    c.CompanyName,
    (
        isnull((
            select sum(od.UnitPrice * od.Quantity * (1 - od.Discount))
    from Orders as o
        join [Order Details] as od on o.OrderID = od.OrderID
    where o.CustomerID = c.CustomerID
        and year(o.OrderDate) = 1996
        ), 0)
        +
        isnull((
            select sum(o.Freight)
    from Orders as o
    where o.CustomerID = c.CustomerID
        and year(o.OrderDate) = 1996
        ), 0)
    ) as "wartość zamówień w 1996"
from
    Customers as c;

-- 3
select
    c.CustomerID,
    c.CompanyName,
    (
    select
        MAX(WartoscZamowienia)
    from
        (
        select
            round(sum(OD.UnitPrice * OD.Quantity * (1 - OD.Discount)), 2) AS WartoscZamowienia
        from
            Orders as o
            join [Order Details] as od on o.OrderID = od.OrderID
        where
          o.CustomerID = c.CustomerID
            and year(O.OrderDate) = 1997
        GROUP BY
          o.OrderID
      ) as SubQueryTable
  ) as "Max wartość w 1997"
from
    Customers as c;
