
# Dokumentowe bazy danych – MongoDB

Ćwiczenie 1


---

**Imiona i nazwiska autorów:** Julian Dworzycki, Radosław Klamka

--- 

## Zadanie 1  - rozwiązanie

```js
use("mydb");

// 1
var q1 = db.employees.find({}).toArray();
console.log("=== Zadanie 1 ===\n" + JSON.stringify(q1, null, 2));

// 2
var q2 = db.employees.find({ "Country": "USA" }).toArray();
console.log("=== Zadanie 2 ===\n" + JSON.stringify(q2, null, 2));

// 3
var q3 = db.employees.insertOne({
    "FirstName": "John",
    "LastName": "Doe",
    "Country": "USA",
    "Age": 30
});
console.log("=== Zadanie 3 ===\n" + JSON.stringify(q3, null, 2));

// 4
var q4 = db.employees.updateOne(
    { "FirstName": "John", "LastName": "Doe" },
    { $set: { "Age": 31 } }
);
console.log("=== Zadanie 4 ===\n" + JSON.stringify(q4, null, 2));

// 5
var q5 = db.employees.updateMany(
    {},
    { $set: { "Photo": ""}}
);
console.log("=== Zadanie 5 ===\n" + JSON.stringify(q5, null, 2));

// 6
var q6 = db.employees.find({}).toArray();
console.log("=== Zadanie 6 ===\n" + JSON.stringify(q6, null, 2));

// 7
var q7 = db.employees.find(
    { "FirstName": "John", "LastName": "Doe" },
).count();
console.log("=== Zadanie 7 (count John Doe) ===\n" + JSON.stringify(q7, null, 2));


// 8
var q8 = db.employees.deleteMany(
    { "FirstName": "John", "LastName": "Doe" },
);
console.log("=== Zadanie 8 ===\n" + JSON.stringify(q8, null, 2));

// 9
var q9 = db.employees.find(
    { "FirstName": "John", "LastName": "Doe" },
).count();
console.log("=== Zadanie 9 (count John Doe) ===\n" + JSON.stringify(q9, null, 2));


// 10
var q10 = db.employees.find({}).toArray();
console.log("=== Zadanie 10 ===\n" + JSON.stringify(q10, null, 2));

// 11
var q11 = db.employees.find(
    { "FirstName": "John", "LastName": "Doe" },
).toArray();
console.log("=== Zadanie 11 ===\n" + JSON.stringify(q11, null, 2));

// 12
var q12 = db.employees.find({}).count();
console.log("=== Zadanie 12 (count) ===\n" + JSON.stringify(q12, null, 2));

// 13
var q13 = db.employees.find(
    {"EmployeeID" : q12}
).toArray();
console.log("=== Zadanie 13 ===\n" + JSON.stringify(q13, null, 2));

// 14
var q14 = db.employees.deleteOne(
    {"EmployeeID" : q12}
);
console.log("=== Zadanie 14 ===\n" + JSON.stringify(q14, null, 2));

// 15
var q15 = db.employees.find(
    {"EmployeeID" : q12}
).toArray();
console.log("=== Zadanie 15 ===\n" + JSON.stringify(q15, null, 2));
```

---
## Zadanie 2  - rozwiązanie


```js
use("mydb");

// 1 — insertMany
var q1 = db.testdata.insertMany([
    { Name: "Alice", Age: 25, Scores: [90, 85, 88], Country: "USA", Created: new Date("2022-01-10") },
    { Name: "Bob", Age: 32, Scores: [70, 75], Country: "Canada", Created: new Date("2023-03-15") },
    { Name: "Charlie", Age: 29, Scores: [95, 92, 91, 89], Country: "USA", Created: new Date("2024-02-01") },
    { Name: "Diana", Age: 40, Scores: [], Country: "Germany", Created: new Date("2021-11-20") }
]);
console.log("=== Zadanie 1 (insertMany) ===\n" + JSON.stringify(q1, null, 2));


// 2 — find with $expr and $size
var q2 = db.testdata.find({
    $expr: { $gt: [ { $size: "$Scores" }, 2 ] }
}).toArray();
console.log("=== Zadanie 2 ($expr + $size) ===\n" + JSON.stringify(q2, null, 2));


// 3 — find with $regex
var q3 = db.testdata.find({
    Name: { $regex: "^A", $options: "i" }
}).toArray();
console.log("=== Zadanie 3 ($regex) ===\n" + JSON.stringify(q3, null, 2));


// 4 — updateMany with $set
var q4 = db.testdata.updateMany(
    {},
    { $set: { Active: true } }
);
console.log("=== Zadanie 4 (updateMany with $set) ===\n" + JSON.stringify(q4, null, 2));


// 5 — updateMany with $inc
var q5 = db.testdata.updateMany(
    { Country: "USA" },
    { $inc: { Age: 1 } }
);
console.log("=== Zadanie 5 (updateMany $inc Age) ===\n" + JSON.stringify(q5, null, 2));


// 6 — find with $expr i $year
var q6 = db.testdata.find({
    $expr: { $eq: [ { $year: "$Created" }, 2023 ] }
}).toArray();
console.log("=== Zadanie 6 ($expr + $year) ===\n" + JSON.stringify(q6, null, 2));


// 7 — count with empty Scores
var q7 = db.testdata.find({ Scores: { $size: 0 } }).count();
console.log("=== Zadanie 7 (count empty Scores) ===\n" + JSON.stringify(q7, null, 2));


// 8 — deleteMany
var q8 = db.testdata.deleteMany({ Country: "Canada" });
console.log("=== Zadanie 8 (deleteMany Canada) ===\n" + JSON.stringify(q8, null, 2));


// 9 — find with $or
var q9 = db.testdata.find({
    $or: [
        { Age: { $lt: 30 } },
        { Country: "Germany" }
    ]
}).toArray();
console.log("=== Zadanie 9 ($or) ===\n" + JSON.stringify(q9, null, 2));


// 10 — find with $expr + $concat
var q10 = db.testdata.find({
    $expr: {
        $regexMatch: {
            input: { $concat: ["$Name", "X"] },
            regex: "^A"
        }
    }
}).toArray();
console.log("=== Zadanie 10 ($expr + $concat + regexMatch) ===\n" + JSON.stringify(q10, null, 2));


// 11 — aggregate: avg age
var q11 = db.testdata.aggregate([
    { $group: { _id: null, avgAge: { $avg: "$Age" } } }
]).toArray();
console.log("=== Zadanie 11 (aggregate avg Age) ===\n" + JSON.stringify(q11, null, 2));


// 12 — count
var q12 = db.testdata.count();
console.log("=== Zadanie 12 (count all) ===\n" + JSON.stringify(q12, null, 2));


// 13 — find where index == count
var q13 = db.testdata.find({ Index: q12 }).toArray();
console.log("=== Zadanie 13 (find Index == count) ===\n" + JSON.stringify(q13, null, 2));


// 14 — deleteOne where Index == count
var q14 = db.testdata.deleteOne({ Index: q12 });
console.log("=== Zadanie 14 (deleteOne Index == count) ===\n" + JSON.stringify(q14, null, 2));


// 15 — find after deletion
var q15 = db.testdata.find({ Index: q12 }).toArray();
console.log("=== Zadanie 15 (verify delete) ===\n" + JSON.stringify(q15, null, 2));
```


---


## Zadanie 3  - rozwiązanie

```js
use("mydb");

// Average salary by country
var avgSalaryByCountry = db.employees.aggregate([
    {
        $group: {
            _id: "$Country",
            count:      { $sum: 1 }
        }
    },
    {
        $project: {
            _id: 0,
            Country: "$_id",
            count: 1
        }
    },
    {
        $sort: { avg_salary: -1 }
    }
]);
print("=== Average salary by country ===");
avgSalaryByCountry.forEach(doc => printjson(doc));

// Employees hired after 1993 with full name, sorted by last name
var employeesHiredAfter1993 = db.employees.aggregate([
    {
        $match: {
            HireDate: { $gte: ISODate("1994-01-01T00:00:00Z") }
        }
    },
    {
        $project: {
            _id: 0,
            FullName: { $concat: ["$FirstName", " ", "$LastName"] },
            Title:    1,
            HireDate: 1,
            Country:  1
        }
    },
    {
        $sort: { FullName: 1 }
    }
]);
print("=== Employees hired after 1993 ===");
employeesHiredAfter1993.forEach(doc => printjson(doc));

// Employees count by title
var employeesCountByTitle = db.employees.aggregate([
    {
        $group: {
            _id:     "$Title",
            cnt:     { $sum: 1 }
        }
    },
    {
        $project: {
            _id:     0,
            Title:   "$_id",
            cnt:     1
        }
    },
    {
        $sort: { cnt: -1 }
    }
]);
print("=== Employees count by title ===");
employeesCountByTitle.forEach(doc => printjson(doc));

// Employees with country (Salary field not available)
var employeesAboveGlobalAvg = db.employees.aggregate([
    {
        $project: {
            _id: 0,
            FullName:  { $concat: ["$FirstName", " ", "$LastName"] },
            Country:   1,
            Title:     1
        }
    },
    {
        $sort: { Country: 1 }
    }
]);
print("=== Employees by country ===");
employeesAboveGlobalAvg.forEach(doc => printjson(doc));

// Employee count by hire year
var employeesByHireYear = db.employees.aggregate([
    {
        $group: {
            _id: { $year: "$HireDate" },
            cnt: { $sum: 1 }
        }
    },
    {
        $project: {
            _id:  0,
            rok:  "$_id",
            cnt:  1
        }
    },
    {
        $sort: { rok: 1 }
    }
]);
print("=== Employee count by hire year ===");
employeesByHireYear.forEach(doc => printjson(doc));

// Age at hire = HireYear - BirthYear
var ageAtHire = db.employees.aggregate([
    {
        $project: {
            _id: 0,
            FullName: { $concat: ["$FirstName", " ", "$LastName"] },
            BirthYear: { $year: "$BirthDate" },
            HireYear:  { $year: "$HireDate" },
            AgeAtHire: {
                $subtract: [
                    { $year: "$HireDate" },
                    { $year: "$BirthDate" }
                ]
            }
        }
    },
    {
        $sort: { AgeAtHire: 1 }
    }
]);
print("=== Age at hire ===");
ageAtHire.forEach(doc => printjson(doc));

// Best employee by country (by hire date)
var topSalaryByCountry = db.employees.aggregate([
    {
        $sort: { "Country": 1, HireDate: -1 }
    },
    {
        $group: {
            _id: "$Country",
            TopEmployee: { $first: { $concat: ["$FirstName", " ", "$LastName"] } },
            Title:       { $first: "$Title" }
        }
    },
    {
        $project: {
            _id:    0,
            Country:     "$_id",
            TopEmployee: 1,
            Title:       1
        }
    },
]);

print("=== Best employee by country ===");
topSalaryByCountry.forEach(doc => printjson(doc));
```

---


## Zadanie 4

- zmień (popraw) strukturę dokumentu w  kolekcji   ` orders_tmp `

- stwórz obiekt/dokument grupujący inf. o adresie wysyłki
	- np.

```js
Shippment : {
	"ShipAddress": "Luisenstr. 48",  
	"ShipCity": "Münster",  
    "ShipCountry": "Germany",  
    "ShipName": "Toms Spezialitäten",  
    "ShipPostalCode": "44087",  
    "ShipRegion": null,  
}
```

- stwórz obiekt/dokument grupujący inf. o datach (zamówienia, wysyłki ... )
	- np.

```js
Dates : {
	"OrderDate": {"$date": "1996-07-04T00:00:00.000Z
	"RequiredDate": {"$date": "1996-08-01T00:00:00.000Z"},	
	"ShippedDate": {"$date": "1996-07-16T00:00:00.000Z"},
}

```

- Dodaj inf. o pełnej wartości całego zamówienia

```js
TotalOrderValue : ...
```

## Zadanie 4  - rozwiązanie

```js
use("mydb");

// Change structure of orders_tmp collection
db.orders.aggregate([
    // Join orderdetails into array Orderdetails
    {
        $lookup: {
            from: "orderdetails",
            localField: "OrderID",
            foreignField: "OrderID",
            as: "Orderdetails"
        }
    },
    // Join shipper details
    {
        $lookup: {
            from: "shippers",
            localField: "ShipVia",
            foreignField: "ShipperID",
            as: "ShipperDetails"
        }
    },
    {
        $addFields: {
            ShipperDetails: {
                $ifNull: [
                    { $arrayElemAt: ["$ShipperDetails", 0] },
                    { ShipperID: "$ShipVia" }
                ]
            }
        }
    },
    // Compute TotalValue for each order line
    {
        $addFields: {
            Orderdetails: {
                $map: {
                    input: "$Orderdetails",
                    as: "od",
                    in: {
                        $mergeObjects: [
                            "$$od",
                            {
                                TotalValue: {
                                    $round: [
                                        { $multiply: ["$$od.UnitPrice", "$$od.Quantity", { $subtract: [1, "$$od.Discount"] }] },
                                        2
                                    ]
                                }
                            }
                        ]
                    }
                }
            }
        }
    },
    // Sum total order value
    {
        $addFields: {
            TotalOrderValue: { $sum: "$Orderdetails.TotalValue" }
        }
    },
    // Group address fields into Shippment object
    {
        $addFields: {
            Shippment: {
                ShipName:       "$ShipName",
                ShipAddress:    "$ShipAddress",
                ShipCity:       "$ShipCity",
                ShipRegion:     "$ShipRegion",
                ShipPostalCode: "$ShipPostalCode",
                ShipCountry:    "$ShipCountry",
                Shipper:        "$ShipperDetails"
            }
        }
    },
    // Group date fields into Dates object
    {
        $addFields: {
            Dates: {
                OrderDate:    "$OrderDate",
                RequiredDate: "$RequiredDate",
                ShippedDate:  "$ShippedDate"
            }
        }
    },
    // Remove original (duplicated) flat fields
    {
        $project: {
            ShipName:       0,
            ShipAddress:    0,
            ShipCity:       0,
            ShipRegion:     0,
            ShipPostalCode: 0,
            ShipCountry:    0,
            Shipper:        0,
            ShipperDetails: 0,
            ShipVia:        0,
            OrderDate:      0,
            RequiredDate:   0,
            ShippedDate:    0
        }
    },
    // Save the result back to orders_tmp (overwrite)
    {
        $out: "orders_tmp"
    }
]);

var orders_tmp = db.orders_tmp.find().limit(2);
print("=== Transformed orders_tmp sample ===");
orders_tmp.forEach(doc => printjson(doc));

var single_order = db.orders_tmp.findOne(
    {},
    {
        OrderID:         1,
        TotalOrderValue: 1,
        "Shippment":     1,
        "Dates":         1
    }
);
print("=== Single transformed order ===");
printjson(single_order);

```

