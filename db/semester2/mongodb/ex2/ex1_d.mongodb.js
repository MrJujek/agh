use("mydb");
const hasCompareUtils = typeof toComparableArray === "function" && typeof deepEqualWithTolerance === "function";

var d_1 = db.orders.aggregate([
    { $lookup: { from: "customers", localField: "CustomerID", foreignField: "CustomerID", as: "Customer" } },
    { $unwind: "$Customer" },
    { $lookup: { from: "orderdetails", localField: "OrderID", foreignField: "OrderID", as: "details" } },
    { $unwind: "$details" },
    {
        $addFields: {
            year: { $substr: ["$OrderDate", 0, 4] },
            month: { $substr: ["$OrderDate", 5, 2] },
            Value: { $multiply: ["$details.UnitPrice", "$details.Quantity", { $subtract: [1, "$details.Discount"] }] }
        }
    },
    {
        $group: {
            _id: { customerId: "$CustomerID", year: "$year", month: "$month" },
            CompanyName: { $first: "$Customer.CompanyName" },
            Total: { $sum: "$Value" }
        }
    },
    {
        $group: {
            _id: "$_id.customerId",
            CustomerID: { $first: "$_id.customerId" },
            CompanyName: { $first: "$CompanyName" },
            Sale: { $push: { Year: "$_id.year", Month: "$_id.month", Total: "$Total" } }
        }
    }
]);

var d_2 = db.OrdersInfo.aggregate([
    {
        $addFields: {
            year: { $substr: ["$Dates.OrderDate", 0, 4] },
            month: { $substr: ["$Dates.OrderDate", 5, 2] }
        }
    },
    {
        $group: {
            _id: { customerId: "$Customer.CustomerID", year: "$year", month: "$month" },
            CompanyName: { $first: "$Customer.CompanyName" },
            Total: { $sum: "$OrderTotal" }
        }
    },
    {
        $group: {
            _id: "$_id.customerId",
            CustomerID: { $first: "$_id.customerId" },
            CompanyName: { $first: "$CompanyName" },
            Sale: { $push: { Year: "$_id.year", Month: "$_id.month", Total: "$Total" } }
        }
    }
]);

var d_3 = db.CustomerInfo.aggregate([
    { $unwind: "$Orders" },
    {
        $addFields: {
            year: { $substr: ["$Orders.Dates.OrderDate", 0, 4] },
            month: { $substr: ["$Orders.Dates.OrderDate", 5, 2] }
        }
    },
    {
        $group: {
            _id: { customerId: "$CustomerID", year: "$year", month: "$month" },
            CompanyName: { $first: "$CompanyName" },
            Total: { $sum: "$Orders.OrderTotal" }
        }
    },
    {
        $group: {
            _id: "$_id.customerId",
            CustomerID: { $first: "$_id.customerId" },
            CompanyName: { $first: "$CompanyName" },
            Sale: { $push: { Year: "$_id.year", Month: "$_id.month", Total: "$Total" } }
        }
    }
]);

if (hasCompareUtils) {
    const a1 = toComparableArray(d_1);
    const a2 = toComparableArray(d_2);
    const a3 = toComparableArray(d_3);

    console.log("====== d ======");
    console.log(a1);

    const EPS = 1e-6;
    print("d1 ~= d2 ?", deepEqualWithTolerance(a1, a2, EPS));
    print("d1 ~= d3 ?", deepEqualWithTolerance(a1, a3, EPS));
    print("d2 ~= d3 ?", deepEqualWithTolerance(a2, a3, EPS));
} else {
    print("Compare utils not found. Printing raw query results only.");
    print("====== d_1 ======");
    printjson(d_1.toArray());
    print("====== d_2 ======");
    printjson(d_2.toArray());
    print("====== d_3 ======");
    printjson(d_3.toArray());
}