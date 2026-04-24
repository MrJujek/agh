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