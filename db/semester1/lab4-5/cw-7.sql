use Northwind;

-- 1
SELECT
    M.FirstName AS ImiePrzelozonego,
    M.LastName AS NazwiskoPrzelozonego,
    P.FirstName AS ImiePodwladnego,
    P.LastName AS NazwiskoPodwladnego
FROM Employees AS M
    JOIN Employees AS P ON M.EmployeeID = P.ReportsTo
ORDER BY NazwiskoPrzelozonego, NazwiskoPodwladnego

-- 2
SELECT
    M.FirstName,
    M.LastName
FROM Employees AS M
    LEFT JOIN Employees AS P ON M.EmployeeID = P.ReportsTo
WHERE P.EmployeeID IS NULL

-- 3
SELECT DISTINCT M.FirstName, M.LastName
FROM Employees AS M
    JOIN Employees AS P ON M.EmployeeID = P.ReportsTo