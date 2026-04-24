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
