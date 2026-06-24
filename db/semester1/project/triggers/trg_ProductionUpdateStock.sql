-- Trigger: ProductionUpdateStock
-- Automatically adds stock when ProductionPlan status changes to 'Zakończone' (3).
CREATE OR ALTER TRIGGER trg_ProductionUpdateStock
ON ProductionPlan
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @DefaultStorageID INT = 1; -- Assuming a default storage for production output

    MERGE INTO ProductStorage AS target
    USING (
        SELECT i.ProductID, COUNT(*) as ProducedCount 
        FROM inserted i
        JOIN deleted d ON i.PlanID = d.PlanID
        WHERE i.StatusID = 3 AND d.StatusID <> 3
        GROUP BY i.ProductID
    ) AS source (ProductID, ProducedQty)
    ON (target.ProductID = source.ProductID AND target.StorageID = @DefaultStorageID)
    WHEN MATCHED THEN
        UPDATE SET Quantity = target.Quantity + source.ProducedQty
    WHEN NOT MATCHED THEN
        INSERT (StorageID, ProductID, Quantity)
        VALUES (@DefaultStorageID, source.ProductID, source.ProducedQty);
END;
GO

/*
-- Example Usage:

-- 1. Complete a production plan (StatusID 3 = 'Zakończone')
-- Assuming PlanID 100 serves ProductID 5
UPDATE ProductionPlan
SET StatusID = 3, EndDate = GETDATE()
WHERE PlanID = 100;

-- 2. Check stock update
SELECT Quantity FROM ProductStorage WHERE ProductID = 5 AND StorageID = 1;
*/

