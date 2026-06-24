-- Procedure: sp_AddOrderDetail
-- Adds a line item to an order.
CREATE OR ALTER PROCEDURE sp_AddOrderDetail
    @OrderID INT,
    @ProductID INT,
    @Quantity INT,
    @Discount FLOAT = 0
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @UnitPrice MONEY;

    -- Get current UnitPrice from Products table
    SELECT @UnitPrice = UnitPrice FROM Products WHERE ProductID = @ProductID;

    IF @UnitPrice IS NULL
    BEGIN
        THROW 50003, 'Product not found.', 1;
    END

    INSERT INTO OrderDetails (OrderID, ProductID, Quantity, Discount, UnitPrice)
    VALUES (@OrderID, @ProductID, @Quantity, @Discount, @UnitPrice);
    
    -- Note: Trigger trg_CheckProductStock will fire here to validate stock.
END;
GO

EXEC sp_AddOrderDetail 
    @OrderID = 500,
    @ProductID = 10,
    @Quantity = 5,
    @Discount = 0.05; -- 5% discount


