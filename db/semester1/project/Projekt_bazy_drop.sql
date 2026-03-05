-- Created by Redgate Data Modeler (https://datamodeler.redgate-platform.com)
-- Last modification date: 2026-01-15 12:39:28.618

-- foreign keys
ALTER TABLE Products DROP CONSTRAINT CategoryID;

ALTER TABLE CategoryVat DROP CONSTRAINT CategoryVat_ProductsCategories;

ALTER TABLE CategoryVat DROP CONSTRAINT CategoryVat_VAT;

ALTER TABLE Cities DROP CONSTRAINT Cities_Regions;

ALTER TABLE Companies DROP CONSTRAINT Company_Customer;

ALTER TABLE WhatSupply DROP CONSTRAINT Connect_Parts;

ALTER TABLE WhatSupply DROP CONSTRAINT Connect_Suppliers;

ALTER TABLE Customers DROP CONSTRAINT Customer_Cities;

ALTER TABLE Employees DROP CONSTRAINT Employees_Cities;

ALTER TABLE Employees DROP CONSTRAINT Employees_Employees;

ALTER TABLE Individuals DROP CONSTRAINT IndividualClient_Customer;

ALTER TABLE OrderDetails DROP CONSTRAINT OrderDetails_Orders;

ALTER TABLE OrderDetails DROP CONSTRAINT OrderDetails_Products;

ALTER TABLE Orders DROP CONSTRAINT Orders_Customer;

ALTER TABLE Orders DROP CONSTRAINT Orders_Employees;

ALTER TABLE Orders DROP CONSTRAINT Orders_OrderStatus;

ALTER TABLE Orders DROP CONSTRAINT Orders_Shippers;

ALTER TABLE PartOrderDetails DROP CONSTRAINT PartOrderDetails_PartOrders;

ALTER TABLE PartOrderDetails DROP CONSTRAINT PartOrderDetails_WhatSupply;

ALTER TABLE PartsStorage DROP CONSTRAINT PartsStorage_Parts;

ALTER TABLE PartsStorage DROP CONSTRAINT PartsStorage_Storage;

ALTER TABLE Parts DROP CONSTRAINT Parts_Categories;

ALTER TABLE ProductLabor DROP CONSTRAINT ProductLabor_LaborTypes;

ALTER TABLE ProductLabor DROP CONSTRAINT ProductLabor_Products;

ALTER TABLE ProductStorage DROP CONSTRAINT ProductStorage_Products;

ALTER TABLE ProductStorage DROP CONSTRAINT ProductStorage_Storage;

ALTER TABLE ProductionPlan DROP CONSTRAINT ProductionPlan_Products;

ALTER TABLE ProductionPlan DROP CONSTRAINT ProductionPlan_StatusTypes;

ALTER TABLE Recipes DROP CONSTRAINT Recipies_Parts;

ALTER TABLE Recipes DROP CONSTRAINT Recipies_Products;

ALTER TABLE Regions DROP CONSTRAINT Regions_Counties;

ALTER TABLE Storage DROP CONSTRAINT Storage_Cities;

-- tables
DROP TABLE CategoryVat;

DROP TABLE Cities;

DROP TABLE Companies;

DROP TABLE Countries;

DROP TABLE Customers;

DROP TABLE Employees;

DROP TABLE Individuals;

DROP TABLE LaborTypes;

DROP TABLE OrderDetails;

DROP TABLE OrderStatus;

DROP TABLE Orders;

DROP TABLE PartOrderDetails;

DROP TABLE PartOrders;

DROP TABLE Parts;

DROP TABLE PartsCategories;

DROP TABLE PartsStorage;

DROP TABLE ProductLabor;

DROP TABLE ProductStorage;

DROP TABLE ProductionPlan;

DROP TABLE ProductionStatus;

DROP TABLE Products;

DROP TABLE ProductsCategories;

DROP TABLE Recipes;

DROP TABLE Regions;

DROP TABLE Shippers;

DROP TABLE Storage;

DROP TABLE Suppliers;

DROP TABLE VAT;

DROP TABLE WhatSupply;

-- End of file.

