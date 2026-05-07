use("mydb");

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