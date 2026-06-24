-- Procedure: sp_GetClientOrdersReport
-- Returns a list of orders for a specific client.
CREATE OR ALTER PROCEDURE sp_GetClientOrdersReport
    @CustomerID INT
AS
BEGIN
    SET NOCOUNT ON;

    SELECT 
        o.OrderID,
        o.OrderDate,
        os.Name AS Status,
        SUM(od.Quantity * od.UnitPrice * (1 - od.Discount)) AS TotalValue
    FROM Orders o
    JOIN OrderStatus os ON o.StatusID = os.StatusID
    LEFT JOIN OrderDetails od ON o.OrderID = od.OrderID
    WHERE o.CustomerID = @CustomerID
    GROUP BY o.OrderID, o.OrderDate, os.Name
    ORDER BY o.OrderDate DESC;
END;
GO

/*
-- Example Usage:

-- Get a report of all orders for Customer 1001
EXEC sp_GetClientOrdersReport @CustomerID = 1001;
*/

