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