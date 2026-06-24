-- Procedure: sp_RestockPart
-- Adds parts to storage (e.g. from Supplier delivery).
CREATE OR ALTER PROCEDURE sp_RestockPart
    @StorageID INT,
    @PartID INT,
    @Quantity INT
AS
BEGIN
    SET NOCOUNT ON;

    IF @Quantity <= 0 THROW 50004, 'Quantity must be positive.', 1;

    MERGE INTO PartsStorage AS target
    USING (SELECT @StorageID AS StorageID, @PartID AS PartID) AS source
    ON (target.StorageID = source.StorageID AND target.PartID = source.PartID)
    WHEN MATCHED THEN
        UPDATE SET Quantity = target.Quantity + @Quantity
    WHEN NOT MATCHED THEN
        INSERT (StorageID, PartID, Quantity)
        VALUES (@StorageID, @PartID, @Quantity);
END;
GO

/*
-- Example Usage:

-- Add 100 units of PartID 15 to StorageID 3
EXEC sp_RestockPart 
    @StorageID = 3,
    @PartID = 15,
    @Quantity = 100;
*/

