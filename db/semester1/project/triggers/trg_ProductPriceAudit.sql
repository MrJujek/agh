-- Trigger: ProductPriceAudit
-- Logs changes to Product UnitPrice.
IF OBJECT_ID('ProductPriceHistory', 'U') IS NULL
BEGIN
    CREATE TABLE ProductPriceHistory (
        HistoryID INT IDENTITY(1,1) PRIMARY KEY,
        ProductID INT,
        OldPrice MONEY,
        NewPrice MONEY,
        ChangeDate DATETIME DEFAULT GETDATE(),
        ChangedBy NVARCHAR(100) DEFAULT SYSTEM_USER
    );
END
GO

CREATE OR ALTER TRIGGER trg_ProductPriceAudit
ON Products
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;

    INSERT INTO ProductPriceHistory (ProductID, OldPrice, NewPrice)
    SELECT d.ProductID, d.UnitPrice, i.UnitPrice
    FROM deleted d
    JOIN inserted i ON d.ProductID = i.ProductID
    WHERE d.UnitPrice <> i.UnitPrice;
END;
GO

/*
-- Example Usage:

-- 1. Update the price of Product 1
UPDATE Products
SET UnitPrice = 25.50
WHERE ProductID = 1;

-- 2. Check the history table
SELECT * FROM ProductPriceHistory WHERE ProductID = 1;
*/

