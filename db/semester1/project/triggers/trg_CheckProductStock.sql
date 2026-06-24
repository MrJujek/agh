-- Trigger: CheckProductStock
-- Checks if there is enough product stock before adding an order detail.
CREATE OR ALTER TRIGGER trg_CheckProductStock
ON OrderDetails
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;

    IF EXISTS (
        SELECT 1
        FROM (
            SELECT i.ProductID, SUM(i.Quantity) as RequestedQty
            FROM inserted i
            GROUP BY i.ProductID
        ) req
        JOIN (
            SELECT ProductID, SUM(Quantity) as AvailableQty
            FROM ProductStorage
            GROUP BY ProductID
        ) stock ON req.ProductID = stock.ProductID
        WHERE req.RequestedQty > stock.AvailableQty
    )
    BEGIN
        RAISERROR ('Insufficient stock for one or more products in the order.', 16, 1);
        ROLLBACK TRANSACTION;
        RETURN;
    END
END;
GO

/*
-- Example Usage:

-- 1. Try to add an order detail for a product with insufficient stock.
-- Assuming ProductID 999 has 0 stock.
INSERT INTO OrderDetails (OrderID, ProductID, Quantity, UnitPrice, Discount)
VALUES (100, 999, 50, 10.00, 0);
-- This should raise error: 'Insufficient stock for one or more products in the order.'
*/

