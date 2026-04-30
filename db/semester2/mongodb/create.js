const { execSync } = require("child_process");
const fs = require("fs");
const path = require("path");

const DB = "mydb";
const DUMP = "./dump/";

console.log("Dropping database...");
execSync(`mongosh --eval "db.getSiblingDB('${DB}').dropDatabase()"`, { stdio: "inherit" });

console.log("Restoring dump...");
execSync(`mongorestore --db ${DB} ${DUMP}`, { stdio: "inherit" });

console.log("\nInserting test data...");

const setupScript = `use("mydb");

db.categories.insertOne({
    CategoryID: 999,
    CategoryName: "Confections",
    Description: "Test category"
});
console.log("✓ Wstawiono kategorię 'Confections'");

db.products.insertOne({
    ProductID: 999,
    ProductName: "Test Candy",
    CategoryID: 999,
    QuantityPerUnit: "1 kg"
});
console.log("✓ Wstawiono produkt 'Test Candy'");

db.customers.insertOne({
    CustomerID: "TEST001",
    CompanyName: "Test Company Ltd",
    City: "TestCity",
    Country: "TestCountry"
});
console.log("✓ Wstawiono klienta 'Test Company Ltd'");

db.orders.insertOne({
    OrderID: 99999,
    CustomerID: "TEST001",
    OrderDate: "1997-06-15",
    RequiredDate: "1997-07-15",
    ShipVia: 1,
    EmployeeID: 1
});
console.log("✓ Wstawiono zamówienie z daty 1997-06-15");

db.orderdetails.insertOne({
    OrderID: 99999,
    ProductID: 999,
    UnitPrice: 100,
    Quantity: 5,
    Discount: 0.1
});
console.log("✓ Wstawiono szczegóły zamówienia (5 szt @ 100 zł, 10% rabat)");
console.log("  Wartość = 100 * 5 * (1 - 0.1) = 450 zł");
`;

const tempFile = path.join(__dirname, "temp-setup.js");
fs.writeFileSync(tempFile, setupScript);

try {
    execSync(`mongosh --db ${DB} ${tempFile}`, { stdio: "inherit" });
} finally {
    fs.unlinkSync(tempFile);
}

console.log("\nDone!");