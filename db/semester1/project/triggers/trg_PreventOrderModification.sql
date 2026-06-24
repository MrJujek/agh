-- Trigger: PreventOrderModification
-- Prevents modification of OrderDetails if the Order is already Shipped (4) or Cancelled (5).
CREATE OR ALTER TRIGGER trg_PreventOrderModification
ON OrderDetails
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
    SET NOCOUNT ON;

    IF EXISTS (
        SELECT 1
        FROM Orders o
        JOIN (
            SELECT DISTINCT OrderID FROM inserted
            UNION
            SELECT DISTINCT OrderID FROM deleted
        ) affected ON o.OrderID = affected.OrderID
        WHERE o.StatusID IN (4, 5) -- 4: Wysłane, 5: Anulowane
    )
    BEGIN
        RAISERROR ('Cannot modify details of an order that is Shipped or Cancelled.', 16, 1);
        ROLLBACK TRANSACTION;
        RETURN;
    END
END;
GO

/*
-- Example Usage:

-- 1. Try to delete an order detail from a Shipped order (StatusID = 4)
DELETE FROM OrderDetails
WHERE OrderID = 200 AND ProductID = 10;
-- This should raise error: 'Cannot modify details of an order that is Shipped or Cancelled.'

-- 2. Try to update quantity in a Cancelled order (StatusID = 5)
UPDATE OrderDetails
SET Quantity = 0
WHERE OrderID = 201;
-- This should raise error: 'Cannot modify details of an order that is Shipped or Cancelled.'
*/

