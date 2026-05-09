# Dokumentowe bazy danych – MongoDB

Ćwiczenie 2


---

**Imiona i nazwiska autorów:** Julian Dworzycki, Radosław Klamka

---

# Zadanie 1 - operacje wyszukiwania danych,  przetwarzanie dokumentów

# a)

stwórz kolekcję  `OrdersInfo`  zawierającą następujące dane o zamówieniach
- kolekcję  `OrdersInfo` należy stworzyć przekształcając dokumenty w oryginalnych kolekcjach `customers, orders, orderdetails, employees, shippers, products, categories, suppliers` do kolekcji  w której pojedynczy dokument opisuje jedno zamówienie

```js
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
var a_check = db.OrdersInfo.find().limit(1).toArray();
console.log("====== a_check ======");
console.log(a_check);
```

Rezultat:
```js
====== a_check ======
[
  {
    _id: ObjectId('63a060b9bb3b972d6f4e2003'),
    OrderID: 10309,
    Customer: {
      CustomerID: 'HUNGO',
      CompanyName: 'Hungry Owl All-Night Grocers',
      City: 'Cork',
      Country: 'Ireland'
    },
    Employee: {
      EmployeeID: 3,
      FirstName: 'Janet',
      LastName: 'Leverling',
      Title: 'Sales Representative'
    },
    Dates: {
      OrderDate: 1996-09-19T00:00:00.000Z,
      RequiredDate: 1996-10-17T00:00:00.000Z
    },
    Orderdetails: [
      {
        UnitPrice: 17.6,
        Quantity: 20,
        Discount: 0,
        Value: 352,
        product: {
          ProductID: 4,
          ProductName: "Chef Anton's Cajun Seasoning",
          QuantityPerUnit: '48 - 6 oz jars',
          CategoryID: 2,
          CategoryName: 'Condiments'
        }
      },
      {
        UnitPrice: 20,
        Quantity: 30,
        Discount: 0,
        Value: 600,
        product: {
          ProductID: 6,
          ProductName: "Grandma's Boysenberry Spread",
          QuantityPerUnit: '12 - 8 oz jars',
          CategoryID: 2,
          CategoryName: 'Condiments'
        }
      },
      {
        UnitPrice: 11.2,
        Quantity: 2,
        Discount: 0,
        Value: 22.4,
        product: {
          ProductID: 42,
          ProductName: 'Singaporean Hokkien Fried Mee',
          QuantityPerUnit: '32 - 1 kg pkgs.',
          CategoryID: 5,
          CategoryName: 'Grains/Cereals'
        }
      },
      {
        UnitPrice: 36.8,
        Quantity: 20,
        Discount: 0,
        Value: 736,
        product: {
          ProductID: 43,
          ProductName: 'Ipoh Coffee',
          QuantityPerUnit: '16 - 500 g tins',
          CategoryID: 1,
          CategoryName: 'Beverages'
        }
      },
      {
        UnitPrice: 17.2,
        Quantity: 3,
        Discount: 0,
        Value: 51.599999999999994,
        product: {
          ProductID: 71,
          ProductName: 'Flotemysost',
          QuantityPerUnit: '10 - 500 g pkgs.',
          CategoryID: 4,
          CategoryName: 'Dairy Products'
        }
      }
    ],
    Freight: 47.3,
    Shipment: {
      Shipper: { ShipperID: 1, CompanyName: 'Speedy Express' },
      ShipName: 'Hungry Owl All-Night Grocers',
      ShipAddress: '8 Johnstown Road',
      ShipCity: 'Cork',
      ShipCountry: 'Ireland'
    },
    OrderTotal: 1762
  }
]
```
# b)

stwórz kolekcję  `CustomerInfo`  zawierającą następujące dane każdym kliencie
- pojedynczy dokument opisuje jednego klienta

```js
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
var b_check = db.CustomerInfo.find().limit(1).toArray();
console.log("====== b_check ======");
console.log(b_check);
```
Rezultat:
```js
====== b_check ======
[
  {
    _id: ObjectId('63a05cdfbb3b972d6f4e097b'),
    CustomerID: 'ALFKI',
    CompanyName: 'Alfreds Futterkiste',
    City: 'Berlin',
    Country: 'Germany',
    Orders: [
      {
        OrderID: 10835,
        Employee: {
          EmployeeID: 1,
          FirstName: 'Nancy',
          LastName: 'Davolio',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1998-01-15T00:00:00.000Z,
          RequiredDate: 1998-02-12T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 55,
            Quantity: 15,
            Discount: 0,
            Value: 825,
            product: {
              ProductID: 59,
              ProductName: 'Raclette Courdavault',
              QuantityPerUnit: '5 kg pkg.',
              CategoryID: 4,
              CategoryName: 'Dairy Products'
            }
          },
          {
            UnitPrice: 13,
            Quantity: 2,
            Discount: 0.20000000298023224,
            Value: 20.799999922513962,
            product: {
              ProductID: 77,
              ProductName: 'Original Frankfurter grüne Soße',
              QuantityPerUnit: '12 boxes',
              CategoryID: 2,
              CategoryName: 'Condiments'
            }
          }
        ],
        Freight: 69.53,
        OrderTotal: 845.799999922514,
        Shipment: {
          Shipper: { ShipperID: 3, CompanyName: 'Federal Shipping' },
          ShipName: "Alfred's Futterkiste",
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      },
      {
        OrderID: 10702,
        Employee: {
          EmployeeID: 4,
          FirstName: 'Margaret',
          LastName: 'Peacock',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1997-10-13T00:00:00.000Z,
          RequiredDate: 1997-11-24T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 10,
            Quantity: 6,
            Discount: 0,
            Value: 60,
            product: {
              ProductID: 3,
              ProductName: 'Aniseed Syrup',
              QuantityPerUnit: '12 - 550 ml bottles',
              CategoryID: 2,
              CategoryName: 'Condiments'
            }
          },
          {
            UnitPrice: 18,
            Quantity: 15,
            Discount: 0,
            Value: 270,
            product: {
              ProductID: 76,
              ProductName: 'Lakkalikööri',
              QuantityPerUnit: '500 ml',
              CategoryID: 1,
              CategoryName: 'Beverages'
            }
          }
        ],
        Freight: 23.94,
        OrderTotal: 330,
        Shipment: {
          Shipper: { ShipperID: 1, CompanyName: 'Speedy Express' },
          ShipName: "Alfred's Futterkiste",
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      },
      {
        OrderID: 10692,
        Employee: {
          EmployeeID: 4,
          FirstName: 'Margaret',
          LastName: 'Peacock',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1997-10-03T00:00:00.000Z,
          RequiredDate: 1997-10-31T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 43.9,
            Quantity: 20,
            Discount: 0,
            Value: 878,
            product: {
              ProductID: 63,
              ProductName: 'Vegie-spread',
              QuantityPerUnit: '15 - 625 g jars',
              CategoryID: 2,
              CategoryName: 'Condiments'
            }
          }
        ],
        Freight: 61.02,
        OrderTotal: 878,
        Shipment: {
          Shipper: { ShipperID: 2, CompanyName: 'United Package' },
          ShipName: "Alfred's Futterkiste",
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      },
      {
        OrderID: 11011,
        Employee: {
          EmployeeID: 3,
          FirstName: 'Janet',
          LastName: 'Leverling',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1998-04-09T00:00:00.000Z,
          RequiredDate: 1998-05-07T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 13.25,
            Quantity: 40,
            Discount: 0.05000000074505806,
            Value: 503.4999996051192,
            product: {
              ProductID: 58,
              ProductName: 'Escargots de Bourgogne',
              QuantityPerUnit: '24 pieces',
              CategoryID: 8,
              CategoryName: 'Seafood'
            }
          },
          {
            UnitPrice: 21.5,
            Quantity: 20,
            Discount: 0,
            Value: 430,
            product: {
              ProductID: 71,
              ProductName: 'Flotemysost',
              QuantityPerUnit: '10 - 500 g pkgs.',
              CategoryID: 4,
              CategoryName: 'Dairy Products'
            }
          }
        ],
        Freight: 1.21,
        OrderTotal: 933.4999996051192,
        Shipment: {
          Shipper: { ShipperID: 1, CompanyName: 'Speedy Express' },
          ShipName: "Alfred's Futterkiste",
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      },
      {
        OrderID: 10643,
        Employee: {
          EmployeeID: 6,
          FirstName: 'Michael',
          LastName: 'Suyama',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1997-08-25T00:00:00.000Z,
          RequiredDate: 1997-09-22T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 45.6,
            Quantity: 15,
            Discount: 0.25,
            Value: 513,
            product: {
              ProductID: 28,
              ProductName: 'Rössle Sauerkraut',
              QuantityPerUnit: '25 - 825 g cans',
              CategoryID: 7,
              CategoryName: 'Produce'
            }
          },
          {
            UnitPrice: 18,
            Quantity: 21,
            Discount: 0.25,
            Value: 283.5,
            product: {
              ProductID: 39,
              ProductName: 'Chartreuse verte',
              QuantityPerUnit: '750 cc per bottle',
              CategoryID: 1,
              CategoryName: 'Beverages'
            }
          },
          {
            UnitPrice: 12,
            Quantity: 2,
            Discount: 0.25,
            Value: 18,
            product: {
              ProductID: 46,
              ProductName: 'Spegesild',
              QuantityPerUnit: '4 - 450 g glasses',
              CategoryID: 8,
              CategoryName: 'Seafood'
            }
          }
        ],
        Freight: 29.46,
        OrderTotal: 814.5,
        Shipment: {
          Shipper: { ShipperID: 1, CompanyName: 'Speedy Express' },
          ShipName: 'Alfreds Futterkiste',
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      },
      {
        OrderID: 10952,
        Employee: {
          EmployeeID: 1,
          FirstName: 'Nancy',
          LastName: 'Davolio',
          Title: 'Sales Representative'
        },
        Dates: {
          OrderDate: 1998-03-16T00:00:00.000Z,
          RequiredDate: 1998-04-27T00:00:00.000Z
        },
        Orderdetails: [
          {
            UnitPrice: 25,
            Quantity: 16,
            Discount: 0.05000000074505806,
            Value: 379.9999997019768,
            product: {
              ProductID: 6,
              ProductName: "Grandma's Boysenberry Spread",
              QuantityPerUnit: '12 - 8 oz jars',
              CategoryID: 2,
              CategoryName: 'Condiments'
            }
          },
          {
            UnitPrice: 45.6,
            Quantity: 2,
            Discount: 0,
            Value: 91.2,
            product: {
              ProductID: 28,
              ProductName: 'Rössle Sauerkraut',
              QuantityPerUnit: '25 - 825 g cans',
              CategoryID: 7,
              CategoryName: 'Produce'
            }
          }
        ],
        Freight: 40.42,
        OrderTotal: 471.19999970197676,
        Shipment: {
          Shipper: { ShipperID: 1, CompanyName: 'Speedy Express' },
          ShipName: "Alfred's Futterkiste",
          ShipAddress: 'Obere Str. 57',
          ShipCity: 'Berlin',
          ShipCountry: 'Germany'
        }
      }
    ]
  }
]
```
# c) 

Napisz polecenie/zapytanie: Dla każdego klienta pokaż wartość zakupionych przez niego produktów z kategorii 'Confections'  w 1997r
- Spróbuj napisać to zapytanie wykorzystując
	- oryginalne kolekcje (`customers, orders, orderdertails, products, categories`)
	- kolekcję `OrderInfo`
	- kolekcję `CustomerInfo`

- porównaj zapytania/polecenia/wyniki
```js
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
```

Wyniki są identyczne dla każdego sposobu. Korzystanie z OrdersInfo oraz CustomerInfo znacznie skraca polecenie, przy czym oba są bardzo do siebie podobne, sięgając do małej liczby dokumentów. Dzięki temu nie marnuje czasu na szukanie powiązań.
```js
====== c_1 ======
[
  {
    _id: 'TEST001',
    CustomerID: 'TEST001',
    CompanyName: 'Test Company Ltd',
    ConfectionsSale97: 450
  }
]
====== c_2 ======
[
  {
    _id: 'TEST001',
    CustomerID: 'TEST001',
    CompanyName: 'Test Company Ltd',
    ConfectionsSale97: 450
  }
]
====== c_3 ======
[
  {
    _id: 'TEST001',
    CustomerID: 'TEST001',
    CompanyName: 'Test Company Ltd',
    ConfectionsSale97: 450
  }
]
```

# d)

Napisz polecenie/zapytanie:  Dla każdego klienta podaje wartość sprzedaży z podziałem na lata i miesiące
Spróbuj napisać to zapytanie wykorzystując
	- oryginalne kolekcje (`customers, orders, orderdertails, products, categories`)
	- kolekcję `OrderInfo`
	- kolekcję `CustomerInfo`

- porównaj zapytania/polecenia/wyniki

```js
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
```

Przykładowy fragment wyników, które są identyczne dla każdego wariantu polecenia. Odniesienie się do OrdersInfo lub CustomersInfo pozwala uniknąć łączenia się z innymi tabelami, dzięki czemu nie marnuje czasu na szukanie powiązań.
```js
====== d_1 ======
[
  {
    _id: 'FAMIA',
    CustomerID: 'FAMIA',
    CompanyName: 'Familia Arquibaldo',
    Sale: [
      { Year: '1997', Month: '01', Total: 224.82999990209936 },
      { Year: '1997', Month: '04', Total: 525.299996316433 },
      { Year: '1996', Month: '12', Total: 166 },
      { Year: '1996', Month: '11', Total: 814.4199954867363 },
      { Year: '1997', Month: '06', Total: 309.99999884516 },
      { Year: '1997', Month: '08', Total: 1779.1999993890522 },
      { Year: '1997', Month: '10', Total: 287.8 }
    ]
  },
  {
    _id: 'WOLZA',
    CustomerID: 'WOLZA',
    CompanyName: 'Wolski  Zajazd',
    Sale: [
      { Year: '1998', Month: '02', Total: 587.5 },
      { Year: '1997', Month: '07', Total: 808 },
      { Year: '1997', Month: '12', Total: 399.85 },
      { Year: '1996', Month: '12', Total: 459 },
      { Year: '1998', Month: '04', Total: 1277.6 }
    ]
  },
  {
    _id: 'LEHMS',
    CustomerID: 'LEHMS',
    CompanyName: 'Lehmanns Marktstand',
    Sale: [
      { Year: '1998', Month: '03', Total: 500 },
      { Year: '1996', Month: '10', Total: 1583.9999999701977 },
      { Year: '1997', Month: '04', Total: 3698.8399949401614 },
      { Year: '1997', Month: '05', Total: 2110.69999922961 },
      { Year: '1997', Month: '07', Total: 2510.86749216523 },
      { Year: '1998', Month: '01', Total: 581 },
      { Year: '1996', Month: '08', Total: 1521.375 },
      { Year: '1997', Month: '06', Total: 1152.5 },
      { Year: '1997', Month: '12', Total: 3603.2200000000003 },
      { Year: '1998', Month: '02', Total: 368.9324997106567 },
      { Year: '1998', Month: '05', Total: 1629.974990323186 }
    ]
  },
...
]
```

# e)

Załóżmy że pojawia się nowe zamówienie dla klienta 'ALFKI',  zawierające dwa produkty 'Chai' oraz "Ikura"
- pozostałe pola w zamówieniu (ceny, liczby sztuk prod, inf o przewoźniku itp. możesz uzupełnić wg własnego uznania)
Napisz polecenie które dodaje takie zamówienie do bazy
- aktualizując oryginalne kolekcje `orders`, `orderdetails`
- aktualizując kolekcję `OrderInfo`
- aktualizując kolekcję `CustomerInfo`

Napisz polecenie 
- aktualizując oryginalną kolekcję orderdetails`
- aktualizując kolekcję `OrderInfo`
- aktualizując kolekcję `CustomerInfo`

```js
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
```

Rezultat:
```js
verification: {
    order: {
      _id: ObjectId('6a007b01eeed8552deeaa928'),
      OrderID: 100000,
      CustomerID: 'ALFKI',
      EmployeeID: 1,
      OrderDate: '2026-05-10T12:33:05.067Z',
      Freight: 10
    },
    orderInfo: {
      _id: ObjectId('6a007b01eeed8552deeaa92b'),
      OrderID: 100001,
      Customer: {
        CustomerID: 'ALFKI',
        CompanyName: 'Alfreds Futterkiste',
        City: 'Berlin',
        Country: 'Germany'
      },
      Employee: { EmployeeID: 1, FirstName: 'Nancy', LastName: 'Davolio' },
      Dates: { OrderDate: '2026-05-10T12:33:05.067Z' },
      Orderdetails: [
        {
          product: { ProductID: 1, ProductName: 'Chai' },
          UnitPrice: 18,
          Quantity: 10,
          Discount: 0,
          Value: 180
        },
        {
          product: { ProductID: 10, ProductName: 'Ikura' },
          UnitPrice: 31,
          Quantity: 5,
          Discount: 0,
          Value: 155
        }
      ],
      Freight: 10,
      OrderTotal: 335
    },
    customerInfo: {
      Orders: [
        {
          OrderID: 100002,
          Dates: { OrderDate: '2026-05-10T12:33:05.067Z' },
          Orderdetails: [
            {
              product: { ProductID: 1, ProductName: 'Chai' },
              UnitPrice: 18,
              Quantity: 10,
              Discount: 0,
              Value: 180
            },
            {
              product: { ProductID: 10, ProductName: 'Ikura' },
              UnitPrice: 31,
              Quantity: 5,
              Discount: 0,
              Value: 155
            }
          ],
          OrderTotal: 335
        }
      ]
    }
  }
```

# f)

Napisz polecenie które modyfikuje zamówienie dodane w pkt e) zwiększając zniżkę o 5% (dla każdej pozycji tego zamówienia) 

Napisz polecenie 
- aktualizując oryginalną kolekcję `orderdetails`
- aktualizując kolekcję `OrderInfo`
- aktualizując kolekcję `CustomerInfo`

```js
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
```

Rezultat:
```js
verification: {
    orderdetails: [
      {
        _id: ObjectId('6a007b01eeed8552deeaa929'),
        OrderID: 100000,
        ProductID: 1,
        UnitPrice: 18,
        Quantity: 10,
        Discount: 0.05
      },
      {
        _id: ObjectId('6a007b01eeed8552deeaa92a'),
        OrderID: 100000,
        ProductID: 10,
        UnitPrice: 31,
        Quantity: 5,
        Discount: 0.05
      }
    ],
    orderInfo: {
      OrderID: 100001,
      Orderdetails: [ { Discount: 0.05 }, { Discount: 0.05 } ]
    },
    customerInfo: {
      Orders: [
        {
          OrderID: 100002,
          Dates: { OrderDate: '2026-05-10T12:33:05.067Z' },
          Orderdetails: [
            {
              product: { ProductID: 1, ProductName: 'Chai' },
              UnitPrice: 18,
              Quantity: 10,
              Discount: 0.05,
              Value: 180
            },
            {
              product: { ProductID: 10, ProductName: 'Ikura' },
              UnitPrice: 31,
              Quantity: 5,
              Discount: 0.05,
              Value: 155
            }
          ],
          OrderTotal: 335
        }
      ]
    }
  }
```

# Zadanie 2 - modelowanie danych


Zaproponuj strukturę bazy danych dla wybranego/przykładowego zagadnienia/problemu

Należy wybrać jedno zagadnienie/problem (A lub B lub C)

Przykład A
- Wykładowcy, przedmioty, studenci, oceny
	- Wykładowcy prowadzą zajęcia z poszczególnych przedmiotów
	- Studenci uczęszczają na zajęcia
	- Wykładowcy wystawiają oceny studentom
	- Studenci oceniają zajęcia

Przykład B
- Firmy, wycieczki, osoby
	- Firmy organizują wycieczki
	- Osoby rezerwują miejsca/wykupują bilety
	- Osoby oceniają wycieczki

Przykład C
- Własny przykład o podobnym stopniu złożoności

a) Zaproponuj  różne warianty struktury bazy danych i dokumentów w poszczególnych kolekcjach oraz przeprowadzić dyskusję każdego wariantu (wskazać wady i zalety każdego z wariantów)
- zdefiniuj schemat/reguły walidacji danych
- wykorzystaj referencje
- dokumenty zagnieżdżone
- tablice

b) Kolekcje należy wypełnić przykładowymi danymi

c) W kontekście zaprezentowania wad/zalet należy zaprezentować kilka przykładów/zapytań/operacji oraz dla których dedykowany jest dany wariant

W sprawozdaniu należy zamieścić przykładowe dokumenty w formacie JSON ( pkt a) i b)), oraz kod zapytań/operacji (pkt c)), wraz z odpowiednim komentarzem opisującym strukturę dokumentów oraz polecenia ilustrujące wykonanie przykładowych operacji na danych

Do sprawozdania należy dołączyć 
- plik z kodem operacji/zapytań w wersji źródłowej (np. plik .js, np. plik .md ) 
- oraz kompletny zrzut wykonanych/przygotowanych baz danych (taki zrzut można wykonać np. za pomocą poleceń `mongoexport`, `mongdump` …)  
	- załącznik ze zrzutem baz powinien mieć format zip

## Zadanie 2  - rozwiązanie

> Wyniki: 
> 
> przykłady, kod, zrzuty ekranów, komentarz ...

```js
--  ...
```
