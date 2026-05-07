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