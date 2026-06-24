-- Procedure: sp_UpdateOrderStatus
-- Updates the status of an order.
CREATE OR ALTER PROCEDURE sp_UpdateOrderStatus
    @OrderID INT,
    @NewStatusID INT
AS
BEGIN
    SET NOCOUNT ON;

    UPDATE Orders
    SET StatusID = @NewStatusID
    WHERE OrderID = @OrderID;

    -- Note: Trigger trg_EnforceStatusTransition will fire here to validate transition.
END;
GO

/*
-- Example Usage:

-- Update Order #500 to Status 2 (e.g., "In Progress")
EXEC sp_UpdateOrderStatus 
    @OrderID = 500,
    @NewStatusID = 2;
*/

