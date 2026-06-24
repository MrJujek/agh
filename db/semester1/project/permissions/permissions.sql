USE [ProjectDB];
GO

-- 1. Tworzenie Ról (Create Roles)

IF NOT EXISTS (SELECT 1 FROM sys.database_principals WHERE name = 'Role_Sales' AND type = 'R')
BEGIN
    CREATE ROLE Role_Sales;
    PRINT 'Utworzono rolę: Role_Sales';
END
GO

IF NOT EXISTS (SELECT 1 FROM sys.database_principals WHERE name = 'Role_Warehouse' AND type = 'R')
BEGIN
    CREATE ROLE Role_Warehouse;
    PRINT 'Utworzono rolę: Role_Warehouse';
END
GO

IF NOT EXISTS (SELECT 1 FROM sys.database_principals WHERE name = 'Role_Manager' AND type = 'R')
BEGIN
    CREATE ROLE Role_Manager;
    PRINT 'Utworzono rolę: Role_Manager';
END
GO

-- 2. Nadawanie Uprawnień (Grant Permissions)

-- =============================================
-- Rola: Role_Sales (Sprzedawca)
-- =============================================

-- Procedury
GRANT EXECUTE ON OBJECT::dbo.sp_AddCustomer TO Role_Sales;
GRANT EXECUTE ON OBJECT::dbo.sp_CreateOrder TO Role_Sales;
GRANT EXECUTE ON OBJECT::dbo.sp_AddOrderDetail TO Role_Sales;
GRANT EXECUTE ON OBJECT::dbo.sp_UpdateOrderStatus TO Role_Sales;
GRANT EXECUTE ON OBJECT::dbo.sp_GetClientOrdersReport TO Role_Sales;

-- Tabele (Dostęp do danych sprzedażowych)
GRANT SELECT, INSERT, UPDATE ON OBJECT::dbo.Customers TO Role_Sales;
GRANT SELECT, INSERT, UPDATE ON OBJECT::dbo.Orders TO Role_Sales;
GRANT SELECT, INSERT, UPDATE ON OBJECT::dbo.OrderDetails TO Role_Sales;
GRANT SELECT ON OBJECT::dbo.Products TO Role_Sales; -- Tylko do odczytu dla sprzedawcy
GRANT SELECT ON OBJECT::dbo.OrderStatus TO Role_Sales;

-- =============================================
-- Rola: Role_Warehouse (Magazynier)
-- =============================================

-- Procedury
GRANT EXECUTE ON OBJECT::dbo.sp_RestockPart TO Role_Warehouse;

-- Tabele (Dostęp do magazynu i produkcji)
GRANT SELECT, INSERT, UPDATE ON OBJECT::dbo.PartsStorage TO Role_Warehouse;
GRANT SELECT, INSERT, UPDATE ON OBJECT::dbo.ProductionPlan TO Role_Warehouse;
GRANT SELECT ON OBJECT::dbo.StockWarnings TO Role_Warehouse; -- Ostrzeżenia
GRANT SELECT ON OBJECT::dbo.Products TO Role_Warehouse;

-- =============================================
-- Rola: Role_Manager (Kierownik)
-- =============================================

-- Kierownik ma dostęp wykonawczy do wszystkich procedur
GRANT EXECUTE ON OBJECT::dbo.sp_AddCustomer TO Role_Manager;
GRANT EXECUTE ON OBJECT::dbo.sp_CreateOrder TO Role_Manager;
GRANT EXECUTE ON OBJECT::dbo.sp_AddOrderDetail TO Role_Manager;
GRANT EXECUTE ON OBJECT::dbo.sp_UpdateOrderStatus TO Role_Manager;
GRANT EXECUTE ON OBJECT::dbo.sp_GetClientOrdersReport TO Role_Manager;
GRANT EXECUTE ON OBJECT::dbo.sp_RestockPart TO Role_Manager;

-- Kierownik ma szeroki dostęp do danych (SELECT na wszystko)
GRANT SELECT ON OBJECT::dbo.Customers TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.Orders TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.OrderDetails TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.Products TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.ProductPriceHistory TO Role_Manager; -- Audyt cen
GRANT SELECT ON OBJECT::dbo.PartsStorage TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.ProductionPlan TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.StockWarnings TO Role_Manager;
GRANT SELECT ON OBJECT::dbo.OrderStatus TO Role_Manager;

PRINT 'Uprawnienia zostały przydzielone.';
GO
