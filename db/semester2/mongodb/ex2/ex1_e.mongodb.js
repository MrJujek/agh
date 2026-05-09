use("mydb");

const orderDate = new Date().toISOString();
const baseOrderID = 99999;

const orderID1 = baseOrderID + 1;
const orderID2 = baseOrderID + 2;
const orderID3 = baseOrderID + 3;

// 1
const result1Order = db.orders.insertOne({
    OrderID: orderID1,
    CustomerID: "ALFKI",
    EmployeeID: 1,
    OrderDate: orderDate,
    Freight: 10
});

const result1Details = db.orderdetails.insertMany([
    { OrderID: orderID1, ProductID: 1, UnitPrice: 18, Quantity: 10, Discount: 0 },
    { OrderID: orderID1, ProductID: 10, UnitPrice: 31, Quantity: 5, Discount: 0 }
]);

// 2
const result2 = db.OrdersInfo.insertOne({
    OrderID: orderID2,
    Customer: { CustomerID: "ALFKI", CompanyName: "Alfreds Futterkiste", City: "Berlin", Country: "Germany" },
    Employee: { EmployeeID: 1, FirstName: "Nancy", LastName: "Davolio" },
    Dates: { OrderDate: orderDate },
    Orderdetails: [
        { product: { ProductID: 1, ProductName: "Chai" }, UnitPrice: 18, Quantity: 10, Discount: 0, Value: 180 },
        { product: { ProductID: 10, ProductName: "Ikura" }, UnitPrice: 31, Quantity: 5, Discount: 0, Value: 155 }
    ],
    Freight: 10,
    OrderTotal: 335
});

// 3
const result3 = db.CustomerInfo.updateOne(
    { CustomerID: "ALFKI" },
    {
        $push: {
            Orders: {
                OrderID: orderID3,
                Dates: { OrderDate: orderDate },
                Orderdetails: [
                    { product: { ProductID: 1, ProductName: "Chai" }, UnitPrice: 18, Quantity: 10, Discount: 0, Value: 180 },
                    { product: { ProductID: 10, ProductName: "Ikura" }, UnitPrice: 31, Quantity: 5, Discount: 0, Value: 155 }
                ],
                OrderTotal: 335
            }
        }
    }
);

const verification = {
    order: db.orders.findOne({ OrderID: orderID1 }),
    orderInfo: db.OrdersInfo.findOne({ OrderID: orderID2 }),
    customerInfo: db.CustomerInfo.findOne(
        { CustomerID: "ALFKI", "Orders.OrderID": orderID3 },
        { _id: 0, "Orders.$": 1 }
    )
};

const results = {
    orderDate,
    orderIDs: { orderID1, orderID2, orderID3 },
    query1: {
        insertOrder: result1Order,
        insertDetails: result1Details
    },
    query2: result2,
    query3: result3,
    verification
};

console.log("====== e ======");
printjson(results);