-- Created by Redgate Data Modeler (https://datamodeler.redgate-platform.com)
-- Last modification date: 2026-01-15 12:39:28.618

-- tables
-- Table: CategoryVat
CREATE TABLE CategoryVat (
    CategoryID int  NOT NULL,
    VatID int  NOT NULL,
    "From" date  NOT NULL,
    "To" date  NULL DEFAULT NULL,
    CONSTRAINT CategoryVat_pk PRIMARY KEY  (CategoryID,VatID)
);

-- Table: Cities
CREATE TABLE Cities (
    CityID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    RegionID int  NOT NULL,
    CONSTRAINT Cities_pk PRIMARY KEY  (CityID)
);

-- Table: Companies
CREATE TABLE Companies (
    CustomerID int  NOT NULL,
    CompanyName varchar(255)  NOT NULL,
    NIP varchar(10)  NOT NULL,
    CONSTRAINT NIPUnique UNIQUE (NIP),
    CONSTRAINT Companies_pk PRIMARY KEY  (CustomerID)
);

-- Table: Countries
CREATE TABLE Countries (
    CountryID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CONSTRAINT Countries_pk PRIMARY KEY  (CountryID)
);

-- Table: Customers
CREATE TABLE Customers (
    CustomerID int  NOT NULL,
    Phone varchar(13)  NOT NULL,
    Email varchar(255)  NOT NULL,
    CityID int  NOT NULL,
    Address varchar(255)  NOT NULL,
    CONSTRAINT Email UNIQUE (Email),
    CONSTRAINT Customers_pk PRIMARY KEY  (CustomerID)
);

-- Table: Employees
CREATE TABLE Employees (
    EmployeeID int  NOT NULL,
    ReportsTo int  NULL,
    CityID int  NOT NULL,
    LastName varchar(255)  NOT NULL,
    FirstName varchar(255)  NOT NULL,
    Title varchar(255)  NOT NULL,
    TitleOfCourtesy varchar(255)  NOT NULL,
    BirthDate date  NOT NULL,
    HireDate date  NOT NULL,
    HomePhone varchar(255)  NOT NULL,
    Notes varchar(255)  NOT NULL,
    Address varchar(255)  NOT NULL,
    CONSTRAINT EmployeCantReportToItself CHECK (ReportsTo <> EmployeeID),
    CONSTRAINT Employees_pk PRIMARY KEY  (EmployeeID)
);

-- Table: Individuals
CREATE TABLE Individuals (
    CustomerID int  NOT NULL,
    Firstname varchar(255)  NOT NULL,
    Lastname varchar(255)  NOT NULL,
    CONSTRAINT Individuals_pk PRIMARY KEY  (CustomerID)
);

-- Table: LaborTypes
CREATE TABLE LaborTypes (
    LaborTypeID int  NOT NULL,
    LaborName varchar(255)  NOT NULL,
    HourlyRate money  NOT NULL,
    CONSTRAINT LaborTypes_pk PRIMARY KEY  (LaborTypeID)
);

-- Table: OrderDetails
CREATE TABLE OrderDetails (
    OrderID int  NOT NULL,
    ProductID int  NOT NULL,
    Quantity int  NOT NULL,
    Discount float(2)  NOT NULL,
    UnitPrice money  NOT NULL,
    CONSTRAINT OrderDetails_pk PRIMARY KEY  (OrderID,ProductID)
);

-- Table: OrderStatus
CREATE TABLE OrderStatus (
    StatusID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CONSTRAINT OrderStatus_pk PRIMARY KEY  (StatusID)
);

-- Table: Orders
CREATE TABLE Orders (
    OrderID int  NOT NULL,
    CustomerID int  NOT NULL,
    EmployeeID int  NOT NULL,
    StatusID int  NOT NULL DEFAULT 1,
    ShipperID int  NOT NULL,
    OrderDate date  NOT NULL,
    RequiredDate date  NOT NULL,
    ShippedDate date  NULL,
    Freight money  NOT NULL,
    CONSTRAINT OrderDateBeforeRequiredDate CHECK (OrderDate < RequiredDate),
    CONSTRAINT Orders_pk PRIMARY KEY  (OrderID)
);

-- Table: PartOrderDetails
CREATE TABLE PartOrderDetails (
    OrderID int  NOT NULL,
    PartID int  NOT NULL,
    SupplierID int  NOT NULL,
    UnitPrice money  NOT NULL,
    Quantity int  NOT NULL,
    CONSTRAINT POUnitPriceNotNegative CHECK (UnitPrice > 0),
    CONSTRAINT POQuantityNotNegative CHECK (Quantity > 0),
    CONSTRAINT PartOrderDetails_pk PRIMARY KEY  (OrderID,PartID)
);

-- Table: PartOrders
CREATE TABLE PartOrders (
    OrderID int  NOT NULL,
    OrderDate date  NOT NULL,
    ShippedDate date  NOT NULL,
    ReceivedDate date  NOT NULL,
    EstimatedArrival date  NOT NULL,
    Freight money  NOT NULL,
    CONSTRAINT POFreightNotNegative CHECK (Freight > 0),
    CONSTRAINT ShippedDateBeforeReceivedDate CHECK (ShippedDate <= ReceivedDate),
    CONSTRAINT OrderDateBeforeShippedDate CHECK (OrderDate <= ShippedDate),
    CONSTRAINT PartOrders_pk PRIMARY KEY  (OrderID)
);

-- Table: Parts
CREATE TABLE Parts (
    PartID int  NOT NULL,
    CategoryID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CONSTRAINT Parts_pk PRIMARY KEY  (PartID)
);

-- Table: PartsCategories
CREATE TABLE PartsCategories (
    CategoryID int  NOT NULL,
    CategoryName varchar(255)  NOT NULL,
    CONSTRAINT PartsCategories_pk PRIMARY KEY  (CategoryID)
);

-- Table: PartsStorage
CREATE TABLE PartsStorage (
    StorageID int  NOT NULL,
    PartID int  NOT NULL,
    Quantity int  NOT NULL,
    CONSTRAINT QuantityNotNegative CHECK (Quantity > 0),
    CONSTRAINT PartsStorage_pk PRIMARY KEY  (StorageID,PartID)
);

-- Table: ProductLabor
CREATE TABLE ProductLabor (
    ProductID int  NOT NULL,
    LaborTypeID int  NOT NULL,
    MinutesOfWork int  NOT NULL,
    HourlyRate money  NOT NULL,
    CONSTRAINT MinutesOfWorkNotNegative CHECK (MinutesOfWork > 0),
    CONSTRAINT HourlyRateNotNegaitve CHECK (HourlyRate > 0),
    CONSTRAINT ProductLabor_pk PRIMARY KEY  (ProductID,LaborTypeID)
);

-- Table: ProductStorage
CREATE TABLE ProductStorage (
    StorageID int  NOT NULL,
    ProductID int  NOT NULL,
    Quantity int  NOT NULL,
    CONSTRAINT ProductStorage_pk PRIMARY KEY  (StorageID,ProductID)
);

-- Table: ProductionPlan
CREATE TABLE ProductionPlan (
    PlanID int  NOT NULL,
    ProductID int  NOT NULL,
    OrderDate date  NOT NULL,
    StartDate date  NOT NULL,
    EndDate date  NOT NULL,
    StatusID int  NOT NULL,
    CONSTRAINT OrderDateBeforeStartDate CHECK (OrderDate <= StartDate),
    CONSTRAINT EndDateAfterStartDate CHECK (StartDate <= EndDate),
    CONSTRAINT ProductionPlan_pk PRIMARY KEY  (PlanID)
);

-- Table: ProductionStatus
CREATE TABLE ProductionStatus (
    StatusID int  NOT NULL,
    StatusName varchar(255)  NOT NULL,
    CONSTRAINT ProductionStatus_pk PRIMARY KEY  (StatusID)
);

-- Table: Products
CREATE TABLE Products (
    ProductID int  NOT NULL,
    UnitPrice money  NOT NULL,
    CategoryID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    ProductionTime int  NOT NULL,
    ProductionCost money  NOT NULL,
    CONSTRAINT ProdUnitPriceNotNegative CHECK (UnitPrice > 0),
    CONSTRAINT ProductionTimeNotNegative CHECK (ProductionTime > 0),
    CONSTRAINT ProductionCostNotNegative CHECK (ProductionCost > 0),
    CONSTRAINT Products_pk PRIMARY KEY  (ProductID)
);

-- Table: ProductsCategories
CREATE TABLE ProductsCategories (
    CategoryID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CONSTRAINT ProductsCategories_pk PRIMARY KEY  (CategoryID)
);

-- Table: Recipes
CREATE TABLE Recipes (
    ProductID int  NOT NULL,
    PartID int  NOT NULL,
    Count int  NOT NULL,
    CONSTRAINT CountNotNegative CHECK (Count > 0),
    CONSTRAINT Recipes_pk PRIMARY KEY  (ProductID,PartID)
);

-- Table: Regions
CREATE TABLE Regions (
    RegionID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CountryID int  NOT NULL,
    CONSTRAINT Regions_pk PRIMARY KEY  (RegionID)
);

-- Table: Shippers
CREATE TABLE Shippers (
    ShipperID int  NOT NULL,
    Name varchar(255)  NOT NULL,
    CONSTRAINT Shippers_pk PRIMARY KEY  (ShipperID)
);

-- Table: Storage
CREATE TABLE Storage (
    StorageID int  NOT NULL,
    CityID int  NOT NULL,
    Address varchar(255)  NOT NULL,
    CONSTRAINT Storage_pk PRIMARY KEY  (StorageID)
);

-- Table: Suppliers
CREATE TABLE Suppliers (
    SupplierID int  NOT NULL,
    CompanyName varchar(255)  NOT NULL,
    Phone varchar(13)  NOT NULL,
    CONSTRAINT Suppliers_pk PRIMARY KEY  (SupplierID)
);

-- Table: VAT
CREATE TABLE VAT (
    VatID int  NOT NULL,
    Amount int  NOT NULL,
    CONSTRAINT VAT_pk PRIMARY KEY  (VatID)
);

-- Table: WhatSupply
CREATE TABLE WhatSupply (
    PartID int  NOT NULL,
    SupplierID int  NOT NULL,
    Price money  NOT NULL,
    DeliveryTime int  NOT NULL,
    CONSTRAINT SPriceNotNegative CHECK (Price > 0),
    CONSTRAINT SDeliveryTimeNotNegative CHECK (DeliveryTime > 0),
    CONSTRAINT WhatSupply_pk PRIMARY KEY  (PartID,SupplierID)
);

-- foreign keys
-- Reference: CategoryID (table: Products)
ALTER TABLE Products ADD CONSTRAINT CategoryID
    FOREIGN KEY (CategoryID)
    REFERENCES ProductsCategories (CategoryID);

-- Reference: CategoryVat_ProductsCategories (table: CategoryVat)
ALTER TABLE CategoryVat ADD CONSTRAINT CategoryVat_ProductsCategories
    FOREIGN KEY (CategoryID)
    REFERENCES ProductsCategories (CategoryID);

-- Reference: CategoryVat_VAT (table: CategoryVat)
ALTER TABLE CategoryVat ADD CONSTRAINT CategoryVat_VAT
    FOREIGN KEY (VatID)
    REFERENCES VAT (VatID);

-- Reference: Cities_Regions (table: Cities)
ALTER TABLE Cities ADD CONSTRAINT Cities_Regions
    FOREIGN KEY (RegionID)
    REFERENCES Regions (RegionID);

-- Reference: Company_Customer (table: Companies)
ALTER TABLE Companies ADD CONSTRAINT Company_Customer
    FOREIGN KEY (CustomerID)
    REFERENCES Customers (CustomerID);

-- Reference: Connect_Parts (table: WhatSupply)
ALTER TABLE WhatSupply ADD CONSTRAINT Connect_Parts
    FOREIGN KEY (PartID)
    REFERENCES Parts (PartID);

-- Reference: Connect_Suppliers (table: WhatSupply)
ALTER TABLE WhatSupply ADD CONSTRAINT Connect_Suppliers
    FOREIGN KEY (SupplierID)
    REFERENCES Suppliers (SupplierID);

-- Reference: Customer_Cities (table: Customers)
ALTER TABLE Customers ADD CONSTRAINT Customer_Cities
    FOREIGN KEY (CityID)
    REFERENCES Cities (CityID);

-- Reference: Employees_Cities (table: Employees)
ALTER TABLE Employees ADD CONSTRAINT Employees_Cities
    FOREIGN KEY (CityID)
    REFERENCES Cities (CityID);

-- Reference: Employees_Employees (table: Employees)
ALTER TABLE Employees ADD CONSTRAINT Employees_Employees
    FOREIGN KEY (ReportsTo)
    REFERENCES Employees (EmployeeID);

-- Reference: IndividualClient_Customer (table: Individuals)
ALTER TABLE Individuals ADD CONSTRAINT IndividualClient_Customer
    FOREIGN KEY (CustomerID)
    REFERENCES Customers (CustomerID);

-- Reference: OrderDetails_Orders (table: OrderDetails)
ALTER TABLE OrderDetails ADD CONSTRAINT OrderDetails_Orders
    FOREIGN KEY (OrderID)
    REFERENCES Orders (OrderID);

-- Reference: OrderDetails_Products (table: OrderDetails)
ALTER TABLE OrderDetails ADD CONSTRAINT OrderDetails_Products
    FOREIGN KEY (ProductID)
    REFERENCES Products (ProductID);

-- Reference: Orders_Customer (table: Orders)
ALTER TABLE Orders ADD CONSTRAINT Orders_Customer
    FOREIGN KEY (CustomerID)
    REFERENCES Customers (CustomerID);

-- Reference: Orders_Employees (table: Orders)
ALTER TABLE Orders ADD CONSTRAINT Orders_Employees
    FOREIGN KEY (EmployeeID)
    REFERENCES Employees (EmployeeID);

-- Reference: Orders_OrderStatus (table: Orders)
ALTER TABLE Orders ADD CONSTRAINT Orders_OrderStatus
    FOREIGN KEY (StatusID)
    REFERENCES OrderStatus (StatusID);

-- Reference: Orders_Shippers (table: Orders)
ALTER TABLE Orders ADD CONSTRAINT Orders_Shippers
    FOREIGN KEY (ShipperID)
    REFERENCES Shippers (ShipperID);

-- Reference: PartOrderDetails_PartOrders (table: PartOrderDetails)
ALTER TABLE PartOrderDetails ADD CONSTRAINT PartOrderDetails_PartOrders
    FOREIGN KEY (OrderID)
    REFERENCES PartOrders (OrderID);

-- Reference: PartOrderDetails_WhatSupply (table: PartOrderDetails)
ALTER TABLE PartOrderDetails ADD CONSTRAINT PartOrderDetails_WhatSupply
    FOREIGN KEY (PartID,SupplierID)
    REFERENCES WhatSupply (PartID,SupplierID);

-- Reference: PartsStorage_Parts (table: PartsStorage)
ALTER TABLE PartsStorage ADD CONSTRAINT PartsStorage_Parts
    FOREIGN KEY (PartID)
    REFERENCES Parts (PartID);

-- Reference: PartsStorage_Storage (table: PartsStorage)
ALTER TABLE PartsStorage ADD CONSTRAINT PartsStorage_Storage
    FOREIGN KEY (StorageID)
    REFERENCES Storage (StorageID);

-- Reference: Parts_Categories (table: Parts)
ALTER TABLE Parts ADD CONSTRAINT Parts_Categories
    FOREIGN KEY (CategoryID)
    REFERENCES PartsCategories (CategoryID);

-- Reference: ProductLabor_LaborTypes (table: ProductLabor)
ALTER TABLE ProductLabor ADD CONSTRAINT ProductLabor_LaborTypes
    FOREIGN KEY (LaborTypeID)
    REFERENCES LaborTypes (LaborTypeID);

-- Reference: ProductLabor_Products (table: ProductLabor)
ALTER TABLE ProductLabor ADD CONSTRAINT ProductLabor_Products
    FOREIGN KEY (ProductID)
    REFERENCES Products (ProductID);

-- Reference: ProductStorage_Products (table: ProductStorage)
ALTER TABLE ProductStorage ADD CONSTRAINT ProductStorage_Products
    FOREIGN KEY (ProductID)
    REFERENCES Products (ProductID);

-- Reference: ProductStorage_Storage (table: ProductStorage)
ALTER TABLE ProductStorage ADD CONSTRAINT ProductStorage_Storage
    FOREIGN KEY (StorageID)
    REFERENCES Storage (StorageID);

-- Reference: ProductionPlan_Products (table: ProductionPlan)
ALTER TABLE ProductionPlan ADD CONSTRAINT ProductionPlan_Products
    FOREIGN KEY (ProductID)
    REFERENCES Products (ProductID);

-- Reference: ProductionPlan_StatusTypes (table: ProductionPlan)
ALTER TABLE ProductionPlan ADD CONSTRAINT ProductionPlan_StatusTypes
    FOREIGN KEY (StatusID)
    REFERENCES ProductionStatus (StatusID);

-- Reference: Recipies_Parts (table: Recipes)
ALTER TABLE Recipes ADD CONSTRAINT Recipies_Parts
    FOREIGN KEY (PartID)
    REFERENCES Parts (PartID);

-- Reference: Recipies_Products (table: Recipes)
ALTER TABLE Recipes ADD CONSTRAINT Recipies_Products
    FOREIGN KEY (ProductID)
    REFERENCES Products (ProductID);

-- Reference: Regions_Counties (table: Regions)
ALTER TABLE Regions ADD CONSTRAINT Regions_Counties
    FOREIGN KEY (CountryID)
    REFERENCES Countries (CountryID);

-- Reference: Storage_Cities (table: Storage)
ALTER TABLE Storage ADD CONSTRAINT Storage_Cities
    FOREIGN KEY (CityID)
    REFERENCES Cities (CityID);

-- End of file.

