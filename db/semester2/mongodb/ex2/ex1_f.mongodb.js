use("mydb");

const baseOrderID = 99999;
const orderID1 = baseOrderID + 1;
const orderID2 = baseOrderID + 2;
const orderID3 = baseOrderID + 3;

// 1
const result1 = db.orderdetails.updateMany(
    { OrderID: orderID1 },
    { $inc: { Discount: 0.05 } }
);

// 2
const result2 = db.OrdersInfo.updateOne(
    { OrderID: orderID2 },
    { $inc: { "Orderdetails.$[].Discount": 0.05 } }
);

// 3
const result3 = db.CustomerInfo.updateOne(
    { CustomerID: "ALFKI", "Orders.OrderID": orderID3 },
    { $inc: { "Orders.$[order].Orderdetails.$[].Discount": 0.05 } },
    { arrayFilters: [{ "order.OrderID": orderID3 }] }
);

const verification = {
    orderdetails: db.orderdetails.find({ OrderID: orderID1 }).toArray(),
    orderInfo: db.OrdersInfo.findOne(
        { OrderID: orderID2 },
        { _id: 0, OrderID: 1, "Orderdetails.ProductID": 1, "Orderdetails.Discount": 1 }
    ),
    customerInfo: db.CustomerInfo.findOne(
        { CustomerID: "ALFKI", "Orders.OrderID": orderID3 },
        { _id: 0, "Orders.$": 1 }
    )
};

const results = {
    orderIDs: { orderID1, orderID2, orderID3 },
    query1: result1,
    query2: result2,
    query3: result3,
    verification
};

console.log("====== f ======");
printjson(results);