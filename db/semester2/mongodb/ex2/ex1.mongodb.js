use("mydb");

var a = db.orders.aggregate([
    { $lookup: { from: "customers", localField: "CustomerID", foreignField: "CustomerID", as: "CustomerInfo" } },
    { $unwind: { path: "$CustomerInfo", preserveNullAndEmptyArrays: true } },

    { $lookup: { from: "employees", localField: "EmployeeID", foreignField: "EmployeeID", as: "EmployeeInfo" } },
    { $unwind: { path: "$EmployeeInfo", preserveNullAndEmptyArrays: true } },

    { $lookup: { from: "shippers", localField: "ShipVia", foreignField: "ShipperID", as: "ShipperInfo" } },
    { $unwind: { path: "$ShipperInfo", preserveNullAndEmptyArrays: true } },

    { $lookup: { from: "orderdetails", localField: "OrderID", foreignField: "OrderID", as: "Orderdetails" } },
    { $unwind: "$Orderdetails" },

    { $lookup: { from: "products", localField: "Orderdetails.ProductID", foreignField: "ProductID", as: "Orderdetails.product" } },
    { $unwind: "$Orderdetails.product" },
    { $lookup: { from: "categories", localField: "Orderdetails.product.CategoryID", foreignField: "CategoryID", as: "Orderdetails.product.category" } },
    { $unwind: "$Orderdetails.product.category" },

    {
        $group: {
            _id: "$_id",
            OrderID: { $first: "$OrderID" },
            Customer: {
                $first: {
                    CustomerID: "$CustomerInfo.CustomerID",
                    CompanyName: "$CustomerInfo.CompanyName",
                    City: "$CustomerInfo.City",
                    Country: "$CustomerInfo.Country"
                }
            },
            Employee: {
                $first: {
                    EmployeeID: "$EmployeeInfo.EmployeeID",
                    FirstName: "$EmployeeInfo.FirstName",
                    LastName: "$EmployeeInfo.LastName",
                    Title: "$EmployeeInfo.Title"
                }
            },
            Dates: {
                $first: {
                    OrderDate: "$OrderDate",
                    RequiredDate: "$RequiredDate"
                }
            },
            Orderdetails: {
                $push: {
                    UnitPrice: "$Orderdetails.UnitPrice",
                    Quantity: "$Orderdetails.Quantity",
                    Discount: "$Orderdetails.Discount",
                    Value: { $multiply: ["$Orderdetails.UnitPrice", "$Orderdetails.Quantity", { $subtract: [1, "$Orderdetails.Discount"] }] },
                    product: {
                        ProductID: "$Orderdetails.product.ProductID",
                        ProductName: "$Orderdetails.product.ProductName",
                        QuantityPerUnit: "$Orderdetails.product.QuantityPerUnit",
                        CategoryID: "$Orderdetails.product.category.CategoryID",
                        CategoryName: "$Orderdetails.product.category.CategoryName"
                    }
                }
            },
            Freight: { $first: "$Freight" },
            Shipment: {
                $first: {
                    Shipper: { ShipperID: "$ShipperInfo.ShipperID", CompanyName: "$ShipperInfo.CompanyName" },
                    ShipName: "$ShipName", ShipAddress: "$ShipAddress", ShipCity: "$ShipCity", ShipCountry: "$ShipCountry"
                }
            }
        }
    },

    {
        $addFields: {
            OrderTotal: { $sum: "$Orderdetails.Value" }

        }
    },
    {
        $out: "OrdersInfo"
    }
]);
console.log("====== a ======");
console.log(a.toArray());
var a_check = db.OrdersInfo.find().toArray();
console.log("====== a_check ======");
console.log(a_check);

var b = db.customers.aggregate([
    { $lookup: { from: "OrdersInfo", localField: "CustomerID", foreignField: "Customer.CustomerID", as: "Orders" } },
    {
        $project: {
            _id: 1,
            CustomerID: 1,
            CompanyName: 1,
            City: 1,
            Country: 1,
            Orders: {
                $map: {
                    input: "$Orders",
                    as: "order",
                    in: {
                        OrderID: "$$order.OrderID",
                        Employee: "$$order.Employee",
                        Dates: "$$order.Dates",
                        Orderdetails: "$$order.Orderdetails",
                        Freight: "$$order.Freight",
                        OrderTotal: "$$order.OrderTotal",
                        Shipment: "$$order.Shipment"
                    }
                }
            }
        }
    },
    { $out: "CustomerInfo" }
]);
console.log("====== b ======");
console.log(b.toArray());
var b_check = db.CustomerInfo.find().toArray();
console.log("====== b_check ======");
console.log(b_check);

var c = db.customers.aggregate([
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
console.log("====== c ======");
console.log(c.toArray());

var d = db.orders.aggregate([
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
console.log("====== d ======");
console.log(d.toArray());