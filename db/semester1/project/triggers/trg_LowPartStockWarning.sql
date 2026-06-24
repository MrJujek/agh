-- Trigger: LowPartStockWarning
-- Warnings for low stock.
IF OBJECT_ID('StockWarnings', 'U') IS NULL
BEGIN
    CREATE TABLE StockWarnings (
        WarningID INT IDENTITY(1,1) PRIMARY KEY,
        PartID INT,
        Message NVARCHAR(255),
        WarningDate DATETIME DEFAULT GETDATE()
    );
END
GO

CREATE OR ALTER TRIGGER trg_LowPartStockWarning
ON PartsStorage
AFTER UPDATE, INSERT
AS
BEGIN
    SET NOCOUNT ON;

    INSERT INTO StockWarnings (PartID, Message)
    SELECT i.PartID, CONCAT('Low stock warning: Part ', i.PartID, ' in Storage ', i.StorageID, ' is at ', i.Quantity)
    FROM inserted i
    WHERE i.Quantity < 50; 
END;
GO

/*
-- Example Usage:

-- 1. Update stock of Part 10 in Storage 2 to a low value (e.g., 5)
UPDATE PartsStorage
SET Quantity = 5
WHERE StorageID = 2 AND PartID = 10;

-- 2. Check the warnings table
SELECT * FROM StockWarnings;
*/

