use("mydb");

// Change structure of orders_tmp collection
db.orders.aggregate([
    // Join orderdetails into array Orderdetails
    {
        $lookup: {
            from: "orderdetails",
            localField: "OrderID",
            foreignField: "OrderID",
            as: "Orderdetails"
        }
    },
    // Join shipper details
    {
        $lookup: {
            from: "shippers",
            localField: "ShipVia",
            foreignField: "ShipperID",
            as: "ShipperDetails"
        }
    },
    {
        $addFields: {
            ShipperDetails: {
                $ifNull: [
                    { $arrayElemAt: ["$ShipperDetails", 0] },
                    { ShipperID: "$ShipVia" }
                ]
            }
        }
    },
    // Compute TotalValue for each order line
    {
        $addFields: {
            Orderdetails: {
                $map: {
                    input: "$Orderdetails",
                    as: "od",
                    in: {
                        $mergeObjects: [
                            "$$od",
                            {
                                TotalValue: {
                                    $round: [
                                        { $multiply: ["$$od.UnitPrice", "$$od.Quantity", { $subtract: [1, "$$od.Discount"] }] },
                                        2
                                    ]
                                }
                            }
                        ]
                    }
                }
            }
        }
    },
    // Sum total order value
    {
        $addFields: {
            TotalOrderValue: { $sum: "$Orderdetails.TotalValue" }
        }
    },
    // Group address fields into Shippment object
    {
        $addFields: {
            Shippment: {
                ShipName:       "$ShipName",
                ShipAddress:    "$ShipAddress",
                ShipCity:       "$ShipCity",
                ShipRegion:     "$ShipRegion",
                ShipPostalCode: "$ShipPostalCode",
                ShipCountry:    "$ShipCountry",
                Shipper:        "$ShipperDetails"
            }
        }
    },
    // Group date fields into Dates object
    {
        $addFields: {
            Dates: {
                OrderDate:    "$OrderDate",
                RequiredDate: "$RequiredDate",
                ShippedDate:  "$ShippedDate"
            }
        }
    },
    // Remove original (duplicated) flat fields
    {
        $project: {
            ShipName:       0,
            ShipAddress:    0,
            ShipCity:       0,
            ShipRegion:     0,
            ShipPostalCode: 0,
            ShipCountry:    0,
            Shipper:        0,
            ShipperDetails: 0,
            ShipVia:        0,
            OrderDate:      0,
            RequiredDate:   0,
            ShippedDate:    0
        }
    },
    // Save the result back to orders_tmp (overwrite)
    {
        $out: "orders_tmp"
    }
]);

var orders_tmp = db.orders_tmp.find().limit(2);
print("=== Transformed orders_tmp sample ===");
orders_tmp.forEach(doc => printjson(doc));

var single_order = db.orders_tmp.findOne(
    {},
    {
        OrderID:         1,
        TotalOrderValue: 1,
        "Shippment":     1,
        "Dates":         1
    }
);
print("=== Single transformed order ===");
printjson(single_order);
