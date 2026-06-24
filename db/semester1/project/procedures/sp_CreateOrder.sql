-- Procedure: sp_CreateOrder
-- Creates a new order header.
CREATE OR ALTER PROCEDURE sp_CreateOrder
    @OrderID INT,
    @CustomerID INT,
    @EmployeeID INT,
    @ShipperID INT,
    @RequiredDate DATE,
    @Freight MONEY,
    @OrderDate DATE = NULL -- Defaults to today if null
AS
BEGIN
    SET NOCOUNT ON;
    
    IF @OrderDate IS NULL SET @OrderDate = GETDATE();

    INSERT INTO Orders (OrderID, CustomerID, EmployeeID, StatusID, ShipperID, OrderDate, RequiredDate, Freight)
    VALUES (@OrderID, @CustomerID, @EmployeeID, 1, @ShipperID, @OrderDate, @RequiredDate, @Freight);
    -- StatusID 1 = Nowe (assuming based on triggers context)
END;
GO

/*
-- Example Usage:

-- Create a new order for Customer 1001, handled by Employee 5, shipped by Shipper 2
EXEC sp_CreateOrder 
    @OrderID = 500,
    @CustomerID = 1001,
    @EmployeeID = 5,
    @ShipperID = 2,
    @RequiredDate = '2023-12-31',
    @Freight = 25.50;
*/

