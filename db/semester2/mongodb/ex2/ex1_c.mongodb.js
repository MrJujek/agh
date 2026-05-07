use("mydb");
const hasCompareUtils = typeof toComparableArray === "function" && typeof deepEqualWithTolerance === "function";

var c_1 = db.customers.aggregate([
    { $lookup: { from: "orders", localField: "CustomerID", foreignField: "CustomerID", as: "order" } },
    { $unwind: "$order" },
    { $match: { "order.OrderDate": { $regex: "^1997" } } },
    { $lookup: { from: "orderdetails", localField: "order.OrderID", foreignField: "OrderID", as: "detail" } },
    { $unwind: "$detail" },
    { $lookup: { from: "products", localField: "detail.ProductID", foreignField: "ProductID", as: "product" } },
    { $unwind: "$product" },
    { $lookup: { from: "categories", localField: "product.CategoryID", foreignField: "CategoryID", as: "category" } },
    { $unwind: "$category" },
    { $match: { "category.CategoryName": "Confections" } },
    {
        $group: {
            _id: "$CustomerID",
            CustomerID: { $first: "$CustomerID" },
            CompanyName: { $first: "$CompanyName" },
            ConfectionsSale97: { $sum: { $multiply: ["$detail.UnitPrice", "$detail.Quantity", { $subtract: [1, "$detail.Discount"] }] } }
        }
    }
])

var c_2 = db.OrdersInfo.aggregate([
    { $match: { "Dates.OrderDate": { $regex: "1997" } } },
    { $unwind: "$Orderdetails" },
    { $match: { "Orderdetails.product.CategoryName": "Confections" } },
    {
        $group: {
            _id: "$Customer.CustomerID",
            CustomerID: { $first: "$Customer.CustomerID" },
            CompanyName: { $first: "$Customer.CompanyName" },
            ConfectionsSale97: { $sum: "$Orderdetails.Value" }
        }
    }
]);

var c_3 = db.CustomerInfo.aggregate([
    { $unwind: "$Orders" },
    { $match: { "Orders.Dates.OrderDate": { $regex: "1997" } } },
    { $unwind: "$Orders.Orderdetails" },
    { $match: { "Orders.Orderdetails.product.CategoryName": "Confections" } },
    {
        $group: {
            _id: "$CustomerID",
            CustomerID: { $first: "$CustomerID" },
            CompanyName: { $first: "$CompanyName" },
            ConfectionsSale97: { $sum: "$Orders.Orderdetails.Value" }
        }
    }
]);

if (hasCompareUtils) {
    const a1 = toComparableArray(c_1);
    const a2 = toComparableArray(c_2);
    const a3 = toComparableArray(c_3);

    console.log("====== c ======");
    console.log(a1);

    const EPS = 1e-6;
    print("c1 ~= c2 ?", deepEqualWithTolerance(a1, a2, EPS));
    print("c1 ~= c3 ?", deepEqualWithTolerance(a1, a3, EPS));
    print("c2 ~= c3 ?", deepEqualWithTolerance(a2, a3, EPS));
} else {
    print("Compare utils not found. Printing raw query results only.");
    print("====== c_1 ======");
    printjson(c_1.toArray());
    print("====== c_2 ======");
    printjson(c_2.toArray());
    print("====== c_3 ======");
    printjson(c_3.toArray());
}