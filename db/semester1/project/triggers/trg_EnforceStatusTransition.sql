-- Trigger: EnforceStatusTransition
-- Enforces logical order status flow.
CREATE OR ALTER TRIGGER trg_EnforceStatusTransition
ON Orders
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    -- Prevent moving BACKWARDS from 'Wysłane' (4) to 'Nowe' (1) or 'Weryfikacja' (2)
    -- Prevent moving from 'Anulowane' (5) to anything else (final state)
    IF EXISTS (
        SELECT 1
        FROM deleted d
        JOIN inserted i ON d.OrderID = i.OrderID
        WHERE 
            (d.StatusID = 4 AND i.StatusID IN (1, 2)) -- Sent -> New/Verify
            OR
            (d.StatusID = 5 AND i.StatusID <> 5) -- Cancelled -> Active
    )
    BEGIN
        RAISERROR ('Invalid order status transition.', 16, 1);
        ROLLBACK TRANSACTION;
        RETURN;
    END
END;
GO

/*
-- Example Usage:

-- 1. Try to change status from 'Wysłane' (4) back to 'Nowe' (1)
UPDATE Orders
SET StatusID = 1
WHERE OrderID = 200 AND StatusID = 4;
-- This should raise error: 'Invalid order status transition.'

-- 2. Try to reactivate an 'Anulowane' (5) order
UPDATE Orders
SET StatusID = 2
WHERE OrderID = 201 AND StatusID = 5;
-- This should raise error: 'Invalid order status transition.'
*/

