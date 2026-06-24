-- Procedure: sp_AddCustomer
-- Adds a new customer, ensuring email and phone are unique (if not already enforced by partial constraints) or just simplifying insertion.
CREATE OR ALTER PROCEDURE sp_AddCustomer
    @CustomerID INT,
    @Phone VARCHAR(13),
    @Email VARCHAR(255),
    @CityID INT,
    @Address VARCHAR(255),
    @CompanyName VARCHAR(255) = NULL, -- If company
    @NIP VARCHAR(10) = NULL,          -- If company
    @FirstName VARCHAR(255) = NULL,   -- If individual
    @LastName VARCHAR(255) = NULL     -- If individual
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRY
        BEGIN TRANSACTION;

        -- Check if CustomerID exists
        IF EXISTS (SELECT 1 FROM Customers WHERE CustomerID = @CustomerID)
        BEGIN
            THROW 50001, 'Customer with this ID already exists.', 1;
        END

        INSERT INTO Customers (CustomerID, Phone, Email, CityID, Address)
        VALUES (@CustomerID, @Phone, @Email, @CityID, @Address);

        IF @CompanyName IS NOT NULL
        BEGIN
            INSERT INTO Companies (CustomerID, CompanyName, NIP)
            VALUES (@CustomerID, @CompanyName, @NIP);
        END
        ELSE IF @FirstName IS NOT NULL AND @LastName IS NOT NULL
        BEGIN
            INSERT INTO Individuals (CustomerID, Firstname, Lastname)
            VALUES (@CustomerID, @FirstName, @LastName);
        END
        ELSE
        BEGIN
            THROW 50002, 'Must provide Company or Individual details.', 1;
        END

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        THROW;
    END CATCH
END;
GO

/*
-- Example Usage:

-- 1. Add an Individual Customer
EXEC sp_AddCustomer 
    @CustomerID = 1001,
    @Phone = '123-456-789',
    @Email = 'john.doe@example.com',
    @CityID = 1, 
    @Address = '123 Maple St',
    @FirstName = 'John',
    @LastName = 'Doe';

-- 2. Add a Company Customer
EXEC sp_AddCustomer 
    @CustomerID = 1002,
    @Phone = '987-654-321',
    @Email = 'contact@acme.com',
    @CityID = 2,
    @Address = '456 Industrial Pkwy',
    @CompanyName = 'Acme Corp',
    @NIP = '1234567890';
*/

