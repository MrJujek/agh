# Dokumentowe bazy danych – MongoDB

Ćwiczenie 2

---

**Imiona i nazwiska autorów:** Julian Dworzycki, Radosław Klamka

---

# Zadanie 1 - operacje wyszukiwania danych,  przetwarzanie dokumentów

## a)

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
```json
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
## b)

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
```json
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
## c) 

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
```json
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
## d)

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
```json
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
## e)

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
```json
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
## f)

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
```json
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

Wybraliśmy przykład A:
- Lecturers, courses, students, grades
  - Lecturers run courses
  - Students attend courses
  - Lecturers assign grades to students
  - Students rate courses

## a) Trzy warianty, które rozważaliśmy:

### Wariant A — Hybrydowy zrównoważony

Separate collections for: lecturers (`lecturers`), students (`students`), courses (`courses`), grades (`grades`) and course_ratings (`course_ratings`). Both student and course may keep arrays `course_ids` / `students_ids` on their side.

#### Zalety i wady tego rozwiązania
- *Zalety*: jeden `$lookup` zamiast trzech dla typowych zapytań; dane statyczne (lista zapisanych) blisko przedmiotu; dane dynamiczne (oceny) osobno; skalowalne; naturalnie odzwierciedla sposób użycia systemu (profil studenta, karta przedmiotu, dziennik ocen to trzy różne widoki).

- *Wady*: przy zapisie/wypisaniu studenta trzeba zaktualizować dwa miejsca (przedmiot + student); drobna redundancja `courses_ids` u studenta.

```js
// Lecturers Collection
db.createCollection("lecturers", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "email" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            email: { bsonType: "string" },
            department: { bsonType: "string" }
         }
      }
   }
});

// Students Collection (contains array of course_ids - controlled redundancy)
db.createCollection("students", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "student_id" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            student_id: { bsonType: "string" },
            email: { bsonType: "string" },
            course_ids: {
               bsonType: "array",
               items: { bsonType: "objectId" },
               description: "Array of ObjectIds - enrolled courses"
            }
         }
      }
   }
});

// Courses Collection (contains enrolled_students array - controlled redundancy)
db.createCollection("courses", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "name", "lecturer_id", "enrolled_students" ],
         properties: {
            name: { bsonType: "string" },
            lecturer_id: { bsonType: "objectId" },
            semester: { bsonType: "int" },
            credits: { bsonType: "int" },
            enrolled_students: {
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "student_id", "student_number" ],
                  properties: {
                     student_id: { bsonType: "objectId" },
                     student_number: { bsonType: "string" }
                  }
               }
            }
         }
      }
   }
});

// Grades Collection (separate - dynamic data)
db.createCollection("grades", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "grade", "date" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            grade: { bsonType: "double" },
            date: { bsonType: "date" }
         }
      }
   }
});

// Course Ratings Collection (separate - dynamic data)
db.createCollection("course_ratings", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "rating" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            rating: { bsonType: "int" },
            comment: { bsonType: "string" },
            date: { bsonType: "date" }
         }
      }
   }
});
```

### Wariant B — Czyste referencje (podejście relacyjne)

Five collections without nesting: lecturers (`lecturers`), students (`students`), courses (`courses`), grades (`grades`) and course_ratings (`course_ratings`). The student–course relation can be implemented via an enrollments collection. Documents are linked by `_id`.
#### Zalety i wady tego rozwiązania

- *Zalety*: brak redundancji; zmiana danych w jednym miejscu; prosta walidacja każdej kolekcji z osobna.

- *Wady*: każdy sensowny widok wymaga 2–4 `$lookup`; tracimy główną zaletę dokumentowej bazy danych; agregacje są ciężkie i przez to słabo czytelne; brak indeksów to katastrofa wydajnościowa.

```js
// Lecturers Collection
db.createCollection("lecturers", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "email" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            email: { bsonType: "string" },
            department: { bsonType: "string" }
         }
      }
   }
});

// Students Collection (no courses_ids array)
db.createCollection("students", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "student_id" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            student_id: { bsonType: "string" },
            email: { bsonType: "string" }
         }
      }
   }
});

// Courses Collection (no enrolled_students array)
db.createCollection("courses", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "name", "lecturer_id" ],
         properties: {
            name: { bsonType: "string" },
            lecturer_id: { bsonType: "objectId" },
            semester: { bsonType: "int" },
            credits: { bsonType: "int" }
         }
      }
   }
});

// Enrollments Collection (join table for student-course relationship)
db.createCollection("enrollments", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "enrollment_date" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            enrollment_date: { bsonType: "date" }
         }
      }
   }
});

// Grades Collection
db.createCollection("grades", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "grade", "date" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            grade: { bsonType: "double" },
            date: { bsonType: "date" }
         }
      }
   }
});

// Course Ratings Collection
db.createCollection("course_ratings", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "student_id", "course_id", "rating" ],
         properties: {
            student_id: { bsonType: "objectId" },
            course_id: { bsonType: "objectId" },
            rating: { bsonType: "int" },
            comment: { bsonType: "string" },
            date: { bsonType: "date" }
         }
      }
   }
});
```

### Wariant C — Hybrydowy skrzywiony ku zagnieżdżeniu

Podobnie do wariantu A, ale zamiast osobnej kolekcji `grades` — oceny są zagnieżdżone bezpośrednio w dokumencie przedmiotu jako tablica { `student_id`, `grade`, `date` }. Oceny zajęć zagnieżdżone w dokumencie studenta.

#### Zalety i wady tego rozwiązania

- *Zalety*: karta przedmiotu z ocenami to jeden dokument; dla małych grup łatwy odczyt i zapis oceny.

- *Wady*: dokument przedmiotu rośnie z każdą oceną i opinią — problem przy dużych grupach; aktualizacja pojedynczej oceny wymaga operatora `$[]` z arrayFilters; agregacja "wszystkie oceny studenta" wymaga `$lookup` lub `$unwind` przez wszystkie przedmioty; historia poprawek ocen trudna do śledzenia. W systemie z kilkuset studentami per przedmiot to zły pomysł.

```js
// Lecturers Collection
db.createCollection("lecturers", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "email" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            email: { bsonType: "string" },
            department: { bsonType: "string" }
         }
      }
   }
});

// Students Collection (contains course_ids array + nested course_ratings)
db.createCollection("students", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "first_name", "last_name", "student_id" ],
         properties: {
            first_name: { bsonType: "string" },
            last_name: { bsonType: "string" },
            student_id: { bsonType: "string" },
            email: { bsonType: "string" },
            course_ids: {
               bsonType: "array",
               items: { bsonType: "objectId" }
            },
            course_ratings: { // Nested ratings given by student to courses
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "course_id", "rating" ],
                  properties: {
                     course_id: { bsonType: "objectId" },
                     rating: { bsonType: "int" },
                     comment: { bsonType: "string" },
                     date: { bsonType: "date" }
                  }
               }
            }
         }
      }
   }
});

// Courses Collection (contains enrolled_students array + nested grades)
db.createCollection("courses", {
   validator: {
      $jsonSchema: {
         bsonType: "object",
         required: [ "name", "lecturer_id", "enrolled_students" ],
         properties: {
            name: { bsonType: "string" },
            lecturer_id: { bsonType: "objectId" },
            semester: { bsonType: "int" },
            credits: { bsonType: "int" },
            enrolled_students: {
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "student_id", "student_number" ],
                  properties: {
                     student_id: { bsonType: "objectId" },
                     student_number: { bsonType: "string" }
                  }
               }
            },
            grades: { // Nested grades assigned to students in this course
               bsonType: "array",
               items: {
                  bsonType: "object",
                  required: [ "student_id", "grade", "date" ],
                  properties: {
                     student_id: { bsonType: "objectId" },
                     grade: { bsonType: "double" },
                     date: { bsonType: "date" }
                  }
               }
            }
         }
      }
   }
});
```

## b) Wypełnianie kolekcji

### Wariant A

```js
const lecturerId = new ObjectId();
const studentId1 = new ObjectId();
const studentId2 = new ObjectId();
const courseId1 = new ObjectId();
const courseId2 = new ObjectId();

// Insert lecturer
db.lecturers.insertOne({
   _id: lecturerId,
   first_name: "John",
   last_name: "Smith",
   email: "john.smith@university.edu",
   department: "Mathematics"
});

// Insert students
db.students.insertMany([
   {
      _id: studentId1,
      first_name: "Alice",
      last_name: "Johnson",
      student_id: "S12345",
      email: "alice.johnson@student.edu",
      course_ids: [ courseId1, courseId2 ]
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu",
      course_ids: [ courseId1 ]
   }
]);

// Insert courses
db.courses.insertMany([
   {
      _id: courseId1,
      name: "Discrete Mathematics",
      lecturer_id: lecturerId,
      semester: 1,
      credits: 5,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" },
         { student_id: studentId2, student_number: "S12346" }
      ]
   },
   {
      _id: courseId2,
      name: "Data Structures",
      lecturer_id: lecturerId,
      semester: 2,
      credits: 4,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" }
      ]
   }
]);

// Insert grades
db.grades.insertMany([
   {
      student_id: studentId1,
      course_id: courseId1,
      grade: 4.5,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId2,
      course_id: courseId1,
      grade: 3.8,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId1,
      course_id: courseId2,
      grade: 4.2,
      date: new Date("2025-12-16")
   }
]);

// Insert course ratings
db.course_ratings.insertMany([
   {
      student_id: studentId1,
      course_id: courseId1,
      rating: 5,
      comment: "Excellent course!",
      date: new Date("2025-12-20")
   },
   {
      student_id: studentId2,
      course_id: courseId1,
      rating: 4,
      comment: "Good content",
      date: new Date("2025-12-20")
   }
]);
```

### Wariant B

```js
const lecturerId = new ObjectId();
const studentId1 = new ObjectId();
const studentId2 = new ObjectId();
const courseId = new ObjectId();
const courseId2 = new ObjectId();

// Insert lecturer
db.lecturers.insertOne({
   _id: lecturerId,
   first_name: "John",
   last_name: "Smith",
   email: "john.smith@university.edu",
   department: "Mathematics"
});

// Insert students
db.students.insertMany([
   {
      _id: studentId1,
      first_name: "Alice",
      last_name: "Johnson",
      student_id: "S12345",
      email: "alice.johnson@student.edu"
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu"
   }
]);

// Insert courses
db.courses.insertMany([
   {
      _id: courseId,
      name: "Discrete Mathematics",
      lecturer_id: lecturerId,
      semester: 1,
      credits: 5
   },
   {
      _id: courseId2,
      name: "Linear Algebra",
      lecturer_id: lecturerId,
      semester: 2,
      credits: 5
   }
]);

// Insert enrollments
db.enrollments.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      enrollment_date: new Date("2025-09-01")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      enrollment_date: new Date("2025-09-01")
   }
]);

// Insert grades
db.grades.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      grade: 4.5,
      date: new Date("2025-12-15")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      grade: 3.8,
      date: new Date("2025-12-15")
   }
]);

// Insert course ratings
db.course_ratings.insertMany([
   {
      student_id: studentId1,
      course_id: courseId,
      rating: 5,
      comment: "Excellent course!",
      date: new Date("2025-12-20")
   },
   {
      student_id: studentId2,
      course_id: courseId,
      rating: 4,
      comment: "Good content",
      date: new Date("2025-12-20")
   }
]);
```

### Wariant C

```js
const lecturerId = new ObjectId();
const studentId1 = new ObjectId();
const studentId2 = new ObjectId();
const courseId1 = new ObjectId();
const courseId2 = new ObjectId();

// Insert lecturer
db.lecturers.insertOne({
   _id: lecturerId,
   first_name: "John",
   last_name: "Smith",
   email: "john.smith@university.edu",
   department: "Mathematics"
});

// Insert students
db.students.insertMany([
   {
      _id: studentId1,
      first_name: "Alice",
      last_name: "Johnson",
      student_id: "S12345",
      email: "alice.johnson@student.edu",
      course_ids: [ courseId1, courseId2 ],
      course_ratings: [
         {
            course_id: courseId1,
            rating: 5,
            comment: "Excellent course!",
            date: new Date("2025-12-20")
         },
         {
            course_id: courseId2,
            rating: 4,
            comment: "Good, but challenging",
            date: new Date("2025-12-21")
         }
      ]
   },
   {
      _id: studentId2,
      first_name: "Bob",
      last_name: "Williams",
      student_id: "S12346",
      email: "bob.williams@student.edu",
      course_ids: [ courseId1 ],
      course_ratings: [
         {
            course_id: courseId1,
            rating: 4,
            comment: "Good content",
            date: new Date("2025-12-20")
         }
      ]
   }
]);

// Insert courses with nested grades
db.courses.insertMany([
   {
      _id: courseId1,
      name: "Discrete Mathematics",
      lecturer_id: lecturerId,
      semester: 1,
      credits: 5,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" },
         { student_id: studentId2, student_number: "S12346" }
      ],
      grades: [
         {
            student_id: studentId1,
            grade: 4.5,
            date: new Date("2025-12-15")
         },
         {
            student_id: studentId2,
            grade: 3.8,
            date: new Date("2025-12-15")
         }
      ]
   },
   {
      _id: courseId2,
      name: "Data Structures",
      lecturer_id: lecturerId,
      semester: 2,
      credits: 4,
      enrolled_students: [
         { student_id: studentId1, student_number: "S12345" }
      ],
      grades: [
         {
            student_id: studentId1,
            grade: 4.2,
            date: new Date("2025-12-16")
         }
      ]
   }
]);
```

## c) Zapytania ukazujące wady i zalety danego wariantu

### Wariant A
```js
// Query 1: Get course attendance list (advantage: single document read)
console.log("\n========== ADVANTAGE 1: Course Attendance List (Single Document) ==========");
const query1Result = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(query1Result, null, 2));

// Query 2: Get student enrolled courses (single lookup - efficient)
console.log("\n========== ADVANTAGE 2: Student with Enrolled Courses (Only 1 Lookup) ==========");
const query2Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "courses",
            localField: "course_ids",
            foreignField: "_id",
            as: "course_details"
        }
    },
    {
        $lookup: {
            from: "lecturers",
            localField: "course_details.lecturer_id",
            foreignField: "_id",
            as: "lecturer_info"
        }
    }
]).toArray();
console.log(JSON.stringify(query2Result, null, 2));

// Query 3: Get course ratings (simple queries with references)
console.log("\n========== ADVANTAGE 3: Course Ratings (Simple Structure) ==========");
const query3Result = db.course_ratings.aggregate([
    { $match: { course_id: courseId1 } },
    {
        $lookup: {
            from: "students",
            localField: "student_id",
            foreignField: "_id",
            as: "student_info"
        }
    },
    { $unwind: "$student_info" },
    {
        $lookup: {
            from: "courses",
            localField: "course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            course: "$course_info.name",
            student: { $concat: ["$student_info.first_name", " ", "$student_info.last_name"] },
            rating: 1,
            comment: 1
        }
    }
]).toArray();
console.log(JSON.stringify(query3Result, null, 2));

// ========== DISADVANTAGES OF VARIANT A ==========

// Query 4: Get student grades with course details (requires joins)
console.log("\n========== DISADVANTAGE 1: Student Grades (Requires Joins) ==========");
const query4Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "grades",
            localField: "_id",
            foreignField: "student_id",
            as: "grades"
        }
    },
    { $unwind: "$grades" },
    {
        $lookup: {
            from: "courses",
            localField: "grades.course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            student_name: { $concat: ["$first_name", " ", "$last_name"] },
            course_name: "$course_info.name",
            grade: "$grades.grade"
        }
    }
]).toArray();
console.log(JSON.stringify(query4Result, null, 2));

// Query 5: Get course with all students and their grades (complex aggregation)
console.log("\n========== DISADVANTAGE 2: Course with Students and Grades (Complex) ==========");
const query5Result = db.courses.aggregate([
    { $match: { _id: courseId1 } },
    {
        $lookup: {
            from: "lecturers",
            localField: "lecturer_id",
            foreignField: "_id",
            as: "lecturer"
        }
    },
    { $unwind: "$lecturer" },
    {
        $lookup: {
            from: "grades",
            localField: "_id",
            foreignField: "course_id",
            as: "grades_data"
        }
    },
    {
        $lookup: {
            from: "students",
            let: { student_id: "$enrolled_students.student_id" },
            pipeline: [
                { $match: { $expr: { $in: ["$_id", "$$student_id"] } } }
            ],
            as: "student_details"
        }
    }
]).toArray();
console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Enroll student to course (DISADVANTAGE: requires 2 updates - maintain consistency)
console.log("\n========== DISADVANTAGE 3: Enroll Student to Course (2 Updates Required) ==========");
const enrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $push: { course_ids: courseId2 } }
);
console.log("Update 1 - Add course to student:");
console.log(JSON.stringify(enrollResult1, null, 2));

const enrollResult2 = db.courses.updateOne(
    { _id: courseId2 },
    { $push: { enrolled_students: { student_id: studentId2, student_number: "S12346" } } }
);
console.log("Update 2 - Add student to course (MUST keep both in sync):");
console.log(JSON.stringify(enrollResult2, null, 2));

// Query 7: Unenroll student from course (DISADVANTAGE: requires 2 updates - maintain consistency)
console.log("\n========== DISADVANTAGE 4: Unenroll Student from Course (2 Updates Required) ==========");
const unenrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $pull: { course_ids: courseId1 } }
);
console.log("Update 1 - Remove course from student:");
console.log(JSON.stringify(unenrollResult1, null, 2));

const unenrollResult2 = db.courses.updateOne(
    { _id: courseId1 },
    { $pull: { enrolled_students: { student_id: studentId2 } } }
);
console.log("Update 2 - Remove student from course (MUST keep both in sync):");
console.log(JSON.stringify(unenrollResult2, null, 2));
```

Rezultat:
```json
========== ADVANTAGE 1: Course Attendance List (Single Document) ==========
{
  "_id": "6a00b9163cbd1fa73ace2135",
  "name": "Discrete Mathematics",
  "lecturer_id": "6a00b9163cbd1fa73ace2132",
  "semester": 1,
  "credits": 5,
  "enrolled_students": [
    {
      "student_id": "6a00b9163cbd1fa73ace2133",
      "student_number": "S12345"
    },
    {
      "student_id": "6a00b9163cbd1fa73ace2134",
      "student_number": "S12346"
    }
  ]
}

========== ADVANTAGE 2: Student with Enrolled Courses (Only 1 Lookup) ==========
[
  {
    "_id": "6a00b9163cbd1fa73ace2133",
    "first_name": "Alice",
    "last_name": "Johnson",
    "student_id": "S12345",
    "email": "alice.johnson@student.edu",
    "course_ids": [
      "6a00b9163cbd1fa73ace2135",
      "6a00b9163cbd1fa73ace2136"
    ],
    "course_details": [
      {
        "_id": "6a00b9163cbd1fa73ace2135",
        "name": "Discrete Mathematics",
        "lecturer_id": "6a00b9163cbd1fa73ace2132",
        "semester": 1,
        "credits": 5,
        "enrolled_students": [
          {
            "student_id": "6a00b9163cbd1fa73ace2133",
            "student_number": "S12345"
          },
          {
            "student_id": "6a00b9163cbd1fa73ace2134",
            "student_number": "S12346"
          }
        ]
      },
      {
        "_id": "6a00b9163cbd1fa73ace2136",
        "name": "Data Structures",
        "lecturer_id": "6a00b9163cbd1fa73ace2132",
        "semester": 2,
        "credits": 4,
        "enrolled_students": [
          {
            "student_id": "6a00b9163cbd1fa73ace2133",
            "student_number": "S12345"
          }
        ]
      }
    ],
    "lecturer_info": [
      {
        "_id": "6a00b9163cbd1fa73ace2132",
        "first_name": "John",
        "last_name": "Smith",
        "email": "john.smith@university.edu",
        "department": "Mathematics"
      }
    ]
  }
]

========== ADVANTAGE 3: Course Ratings (Simple Structure) ==========
[
  {
    "rating": 5,
    "comment": "Excellent course!",
    "course": "Discrete Mathematics",
    "student": "Alice Johnson"
  },
  {
    "rating": 4,
    "comment": "Good content",
    "course": "Discrete Mathematics",
    "student": "Bob Williams"
  }
]

========== DISADVANTAGE 1: Student Grades (Requires Joins) ==========
[
  {
    "student_name": "Alice Johnson",
    "course_name": "Discrete Mathematics",
    "grade": 4.5
  },
  {
    "student_name": "Alice Johnson",
    "course_name": "Data Structures",
    "grade": 4.2
  }
]

========== DISADVANTAGE 2: Course with Students and Grades (Complex) ==========
[
  {
    "_id": "6a00b9163cbd1fa73ace2135",
    "name": "Discrete Mathematics",
    "lecturer_id": "6a00b9163cbd1fa73ace2132",
    "semester": 1,
    "credits": 5,
    "enrolled_students": [
      {
        "student_id": "6a00b9163cbd1fa73ace2133",
        "student_number": "S12345"
      },
      {
        "student_id": "6a00b9163cbd1fa73ace2134",
        "student_number": "S12346"
      }
    ],
    "lecturer": {
      "_id": "6a00b9163cbd1fa73ace2132",
      "first_name": "John",
      "last_name": "Smith",
      "email": "john.smith@university.edu",
      "department": "Mathematics"
    },
    "grades_data": [
      {
        "_id": "6a00b9163cbd1fa73ace2137",
        "student_id": "6a00b9163cbd1fa73ace2133",
        "course_id": "6a00b9163cbd1fa73ace2135",
        "grade": 4.5,
        "date": "2025-12-15T00:00:00.000Z"
      },
      {
        "_id": "6a00b9163cbd1fa73ace2138",
        "student_id": "6a00b9163cbd1fa73ace2134",
        "course_id": "6a00b9163cbd1fa73ace2135",
        "grade": 3.8,
        "date": "2025-12-15T00:00:00.000Z"
      }
    ],
    "student_details": [
      {
        "_id": "6a00b9163cbd1fa73ace2133",
        "first_name": "Alice",
        "last_name": "Johnson",
        "student_id": "S12345",
        "email": "alice.johnson@student.edu",
        "course_ids": [
          "6a00b9163cbd1fa73ace2135",
          "6a00b9163cbd1fa73ace2136"
        ]
      },
      {
        "_id": "6a00b9163cbd1fa73ace2134",
        "first_name": "Bob",
        "last_name": "Williams",
        "student_id": "S12346",
        "email": "bob.williams@student.edu",
        "course_ids": [
          "6a00b9163cbd1fa73ace2135"
        ]
      }
    ]
  }
]

========== DISADVANTAGE 3: Enroll Student to Course (2 Updates Required) ==========
Update 1 - Add course to student:
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}
Update 2 - Add student to course (MUST keep both in sync):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

========== DISADVANTAGE 4: Unenroll Student from Course (2 Updates Required) ==========
Update 1 - Remove course from student:
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}
Update 2 - Remove student from course (MUST keep both in sync):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

```

### Wariant B
```js

// Query 1: Enroll student to course (advantage: single insert, no consistency issues)
console.log("\n========== ADVANTAGE 1: Enroll Student to Course (Single Insert) ==========");
const enrollResult = db.enrollments.insertOne({
    student_id: studentId1,
    course_id: courseId2,
    enrollment_date: new Date()
});
console.log("Single Insert - No synchronization needed:");
console.log(JSON.stringify(enrollResult, null, 2));

// Query 2: Unenroll student from course (advantage: single delete, no consistency issues)
console.log("\n========== ADVANTAGE 2: Unenroll Student from Course (Single Delete) ==========");
const unenrollResult = db.enrollments.deleteOne({
    student_id: studentId1,
    course_id: courseId2
});
console.log("Single Delete - No synchronization needed:");
console.log(JSON.stringify(unenrollResult, null, 2));

// Query 3: No redundancy - data in one place
console.log("\n========== ADVANTAGE 3: No Data Redundancy (Pure References) ==========");
console.log("All relations stored in separate, normalized collections:");
console.log("- Students are just student info");
console.log("- Courses are just course info");
console.log("- Enrollments keep the relationship");
console.log("- Grades are separate");
console.log("- Course ratings are separate");

// ========== DISADVANTAGES OF VARIANT B ==========

// Query 4: Get student profile with enrolled courses (many lookups)
console.log("\n========== DISADVANTAGE 1: Student Profile with Enrolled Courses (3 Lookups) ==========");
const query1Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "student_id",
            as: "my_enrollments"
        }
    },
    { $unwind: "$my_enrollments" },
    {
        $lookup: {
            from: "courses",
            localField: "my_enrollments.course_id",
            foreignField: "_id",
            as: "course_details"
        }
    },
    { $unwind: "$course_details" },
    {
        $lookup: {
            from: "lecturers",
            localField: "course_details.lecturer_id",
            foreignField: "_id",
            as: "lecturer_details"
        }
    },
    { $unwind: "$lecturer_details" }
]).toArray();

console.log(JSON.stringify(query1Result, null, 2));

// Query 5: Get student grades for all courses (many lookups)
console.log("\n========== DISADVANTAGE 2: Student Grades for All Courses (Multiple Lookups) ==========");
const query5Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    {
        $lookup: {
            from: "grades",
            localField: "_id",
            foreignField: "student_id",
            as: "grades"
        }
    },
    {
        $lookup: {
            from: "courses",
            localField: "grades.course_id",
            foreignField: "_id",
            as: "course_details"
        }
    }
]).toArray();

console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Get course with all enrolled students and their grades (many lookups)
console.log("\n========== DISADVANTAGE 3: Course with Enrolled Students and Grades (4 Lookups) ==========");
const query6Result = db.courses.aggregate([
    { $match: { _id: courseId } },
    {
        $lookup: {
            from: "lecturers",
            localField: "lecturer_id",
            foreignField: "_id",
            as: "lecturer"
        }
    },
    { $unwind: "$lecturer" },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "course_id",
            as: "enrollments"
        }
    },
    { $unwind: "$enrollments" },
    {
        $lookup: {
            from: "students",
            localField: "enrollments.student_id",
            foreignField: "_id",
            as: "student_info"
        }
    },
    { $unwind: "$student_info" },
    {
        $lookup: {
            from: "grades",
            let: { student: "$student_info._id", course: "$_id" },
            pipeline: [
                { $match: { $expr: { $and: [{ $eq: ["$student_id", "$$student"] }, { $eq: ["$course_id", "$$course"] }] } } }
            ],
            as: "student_grades"
        }
    }
]).toArray();

console.log(JSON.stringify(query6Result, null, 2));

// Query 7: Get course ratings for a course (multiple lookups)
console.log("\n========== DISADVANTAGE 4: Course Ratings (Multiple Lookups) ==========");
const query7Result = db.course_ratings.aggregate([
    { $match: { course_id: courseId } },
    {
        $lookup: {
            from: "students",
            localField: "student_id",
            foreignField: "_id",
            as: "student_info"
        }
    },
    { $unwind: "$student_info" },
    {
        $lookup: {
            from: "courses",
            localField: "course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" }
]).toArray();

console.log(JSON.stringify(query7Result, null, 2));

// Query 8: Get attendance list for a course (many lookups)
console.log("\n========== DISADVANTAGE 5: Course Attendance List (Multiple Lookups) ==========");
const query8Result = db.courses.aggregate([
    { $match: { _id: courseId } },
    {
        $lookup: {
            from: "lecturers",
            localField: "lecturer_id",
            foreignField: "_id",
            as: "lecturer_info"
        }
    },
    { $unwind: "$lecturer_info" },
    {
        $lookup: {
            from: "enrollments",
            localField: "_id",
            foreignField: "course_id",
            as: "enrollments_data"
        }
    },
    {
        $lookup: {
            from: "students",
            let: { student_id: "$enrollments_data.student_id" },
            pipeline: [
                { $match: { $expr: { $in: ["$_id", "$$student_id"] } } }
            ],
            as: "student_list"
        }
    }
]).toArray();

console.log(JSON.stringify(query8Result, null, 2));

// Query 9: Average grade per course (many lookups)
console.log("\n========== DISADVANTAGE 6: Course Average Grade (Aggregation with Lookups) ==========");
const query9Result = db.grades.aggregate([
    { $match: { course_id: courseId } },
    {
        $group: {
            _id: "$course_id",
            average_grade: { $avg: "$grade" },
            total_grades: { $sum: 1 }
        }
    },
    {
        $lookup: {
            from: "courses",
            localField: "_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            course_name: "$course_info.name",
            average_grade: 1,
            total_grades: 1
        }
    }
]).toArray();
console.log(JSON.stringify(query9Result, null, 2));
```

Rezultat:
```json

========== ADVANTAGE 1: Enroll Student to Course (Single Insert) ==========
Single Insert - No synchronization needed:
{
  "acknowledged": true,
  "insertedId": "6a00b951e2019c8480631c10"
}

========== ADVANTAGE 2: Unenroll Student from Course (Single Delete) ==========
Single Delete - No synchronization needed:
{
  "acknowledged": true,
  "deletedCount": 1
}

========== ADVANTAGE 3: No Data Redundancy (Pure References) ==========
All relations stored in separate, normalized collections:
- Students are just student info
- Courses are just course info
- Enrollments keep the relationship
- Grades are separate
- Course ratings are separate

========== DISADVANTAGE 1: Student Profile with Enrolled Courses (3 Lookups) ==========
[
  {
    "_id": "6a00b951e2019c8480631c06",
    "first_name": "Alice",
    "last_name": "Johnson",
    "student_id": "S12345",
    "email": "alice.johnson@student.edu",
    "my_enrollments": {
      "_id": "6a00b951e2019c8480631c0a",
      "student_id": "6a00b951e2019c8480631c06",
      "course_id": "6a00b951e2019c8480631c08",
      "enrollment_date": "2025-09-01T00:00:00.000Z"
    },
    "course_details": {
      "_id": "6a00b951e2019c8480631c08",
      "name": "Discrete Mathematics",
      "lecturer_id": "6a00b951e2019c8480631c05",
      "semester": 1,
      "credits": 5
    },
    "lecturer_details": {
      "_id": "6a00b951e2019c8480631c05",
      "first_name": "John",
      "last_name": "Smith",
      "email": "john.smith@university.edu",
      "department": "Mathematics"
    }
  }
]

========== DISADVANTAGE 2: Student Grades for All Courses (Multiple Lookups) ==========
[
  {
    "_id": "6a00b951e2019c8480631c06",
    "first_name": "Alice",
    "last_name": "Johnson",
    "student_id": "S12345",
    "email": "alice.johnson@student.edu",
    "grades": [
      {
        "_id": "6a00b951e2019c8480631c0c",
        "student_id": "6a00b951e2019c8480631c06",
        "course_id": "6a00b951e2019c8480631c08",
        "grade": 4.5,
        "date": "2025-12-15T00:00:00.000Z"
      }
    ],
    "course_details": [
      {
        "_id": "6a00b951e2019c8480631c08",
        "name": "Discrete Mathematics",
        "lecturer_id": "6a00b951e2019c8480631c05",
        "semester": 1,
        "credits": 5
      }
    ]
  }
]

========== DISADVANTAGE 3: Course with Enrolled Students and Grades (4 Lookups) ==========
[
  {
    "_id": "6a00b951e2019c8480631c08",
    "name": "Discrete Mathematics",
    "lecturer_id": "6a00b951e2019c8480631c05",
    "semester": 1,
    "credits": 5,
    "lecturer": {
      "_id": "6a00b951e2019c8480631c05",
      "first_name": "John",
      "last_name": "Smith",
      "email": "john.smith@university.edu",
      "department": "Mathematics"
    },
    "enrollments": {
      "_id": "6a00b951e2019c8480631c0a",
      "student_id": "6a00b951e2019c8480631c06",
      "course_id": "6a00b951e2019c8480631c08",
      "enrollment_date": "2025-09-01T00:00:00.000Z"
    },
    "student_info": {
      "_id": "6a00b951e2019c8480631c06",
      "first_name": "Alice",
      "last_name": "Johnson",
      "student_id": "S12345",
      "email": "alice.johnson@student.edu"
    },
    "student_grades": [
      {
        "_id": "6a00b951e2019c8480631c0c",
        "student_id": "6a00b951e2019c8480631c06",
        "course_id": "6a00b951e2019c8480631c08",
        "grade": 4.5,
        "date": "2025-12-15T00:00:00.000Z"
      }
    ]
  },
  {
    "_id": "6a00b951e2019c8480631c08",
    "name": "Discrete Mathematics",
    "lecturer_id": "6a00b951e2019c8480631c05",
    "semester": 1,
    "credits": 5,
    "lecturer": {
      "_id": "6a00b951e2019c8480631c05",
      "first_name": "John",
      "last_name": "Smith",
      "email": "john.smith@university.edu",
      "department": "Mathematics"
    },
    "enrollments": {
      "_id": "6a00b951e2019c8480631c0b",
      "student_id": "6a00b951e2019c8480631c07",
      "course_id": "6a00b951e2019c8480631c08",
      "enrollment_date": "2025-09-01T00:00:00.000Z"
    },
    "student_info": {
      "_id": "6a00b951e2019c8480631c07",
      "first_name": "Bob",
      "last_name": "Williams",
      "student_id": "S12346",
      "email": "bob.williams@student.edu"
    },
    "student_grades": [
      {
        "_id": "6a00b951e2019c8480631c0d",
        "student_id": "6a00b951e2019c8480631c07",
        "course_id": "6a00b951e2019c8480631c08",
        "grade": 3.8,
        "date": "2025-12-15T00:00:00.000Z"
      }
    ]
  }
]

========== DISADVANTAGE 4: Course Ratings (Multiple Lookups) ==========
[
  {
    "_id": "6a00b951e2019c8480631c0e",
    "student_id": "6a00b951e2019c8480631c06",
    "course_id": "6a00b951e2019c8480631c08",
    "rating": 5,
    "comment": "Excellent course!",
    "date": "2025-12-20T00:00:00.000Z",
    "student_info": {
      "_id": "6a00b951e2019c8480631c06",
      "first_name": "Alice",
      "last_name": "Johnson",
      "student_id": "S12345",
      "email": "alice.johnson@student.edu"
    },
    "course_info": {
      "_id": "6a00b951e2019c8480631c08",
      "name": "Discrete Mathematics",
      "lecturer_id": "6a00b951e2019c8480631c05",
      "semester": 1,
      "credits": 5
    }
  },
  {
    "_id": "6a00b951e2019c8480631c0f",
    "student_id": "6a00b951e2019c8480631c07",
    "course_id": "6a00b951e2019c8480631c08",
    "rating": 4,
    "comment": "Good content",
    "date": "2025-12-20T00:00:00.000Z",
    "student_info": {
      "_id": "6a00b951e2019c8480631c07",
      "first_name": "Bob",
      "last_name": "Williams",
      "student_id": "S12346",
      "email": "bob.williams@student.edu"
    },
    "course_info": {
      "_id": "6a00b951e2019c8480631c08",
      "name": "Discrete Mathematics",
      "lecturer_id": "6a00b951e2019c8480631c05",
      "semester": 1,
      "credits": 5
    }
  }
]

========== DISADVANTAGE 5: Course Attendance List (Multiple Lookups) ==========
[
  {
    "_id": "6a00b951e2019c8480631c08",
    "name": "Discrete Mathematics",
    "lecturer_id": "6a00b951e2019c8480631c05",
    "semester": 1,
    "credits": 5,
    "lecturer_info": {
      "_id": "6a00b951e2019c8480631c05",
      "first_name": "John",
      "last_name": "Smith",
      "email": "john.smith@university.edu",
      "department": "Mathematics"
    },
    "enrollments_data": [
      {
        "_id": "6a00b951e2019c8480631c0a",
        "student_id": "6a00b951e2019c8480631c06",
        "course_id": "6a00b951e2019c8480631c08",
        "enrollment_date": "2025-09-01T00:00:00.000Z"
      },
      {
        "_id": "6a00b951e2019c8480631c0b",
        "student_id": "6a00b951e2019c8480631c07",
        "course_id": "6a00b951e2019c8480631c08",
        "enrollment_date": "2025-09-01T00:00:00.000Z"
      }
    ],
    "student_list": [
      {
        "_id": "6a00b951e2019c8480631c06",
        "first_name": "Alice",
        "last_name": "Johnson",
        "student_id": "S12345",
        "email": "alice.johnson@student.edu"
      },
      {
        "_id": "6a00b951e2019c8480631c07",
        "first_name": "Bob",
        "last_name": "Williams",
        "student_id": "S12346",
        "email": "bob.williams@student.edu"
      }
    ]
  }
]

========== DISADVANTAGE 6: Course Average Grade (Aggregation with Lookups) ==========
[
  {
    "average_grade": 4.15,
    "total_grades": 2,
    "course_name": "Discrete Mathematics"
  }
]
```

### Wariant C
```js

// Query 1: Get course with all grades in one document (advantage)
console.log("\n========== ADVANTAGE 1: Course Card with All Grades (Single Document) ==========");
const query1Result = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(query1Result, null, 2));

// Query 2: Get student ratings (nested in student - easy access)
console.log("\n========== ADVANTAGE 2: Student Course Ratings (Nested in Student) ==========");
const query2Result = db.students.aggregate([
    { $match: { _id: studentId1 } },
    { $unwind: "$course_ratings" },
    {
        $lookup: {
            from: "courses",
            localField: "course_ratings.course_id",
            foreignField: "_id",
            as: "course_info"
        }
    },
    { $unwind: "$course_info" },
    {
        $project: {
            _id: 0,
            student_name: { $concat: ["$first_name", " ", "$last_name"] },
            course_name: "$course_info.name",
            rating: "$course_ratings.rating",
            comment: "$course_ratings.comment",
            date: "$course_ratings.date"
        }
    }
]).toArray();
console.log(JSON.stringify(query2Result, null, 2));

// Query 3: Add a new grade to course (easy - just push)
console.log("\n========== ADVANTAGE 3: Adding a New Grade to Course (Simple Push) ==========");
const addGradeResult = db.courses.updateOne(
    { _id: courseId2 },
    {
        $push: {
            grades: {
                student_id: studentId2,
                grade: 3.9,
                date: new Date("2025-12-16")
            }
        }
    }
);
console.log("Add Grade Result (single operation):");
console.log(JSON.stringify(addGradeResult, null, 2));

// Verify grades were added
console.log("\nVerification - Updated Grades in Course:");
const courseWithNewGrade = db.courses.findOne(
    { _id: courseId2 },
    { name: 1, grades: 1 }
);
console.log(JSON.stringify(courseWithNewGrade, null, 2));

// Query 4: Add a new course rating to student (simple push)
console.log("\n========== ADVANTAGE 4: Adding a New Course Rating to Student (Simple Push) ==========");
const addRatingResult = db.students.updateOne(
    { _id: studentId2 },
    {
        $push: {
            course_ratings: {
                course_id: courseId2,
                rating: 3,
                comment: "Interesting but difficult",
                date: new Date("2025-12-22")
            }
        }
    }
);
console.log("Add Rating Result (single operation):");
console.log(JSON.stringify(addRatingResult, null, 2));

// Verify ratings were added
console.log("\nVerification - Updated Student with New Rating:");
const studentWithNewRating = db.students.findOne(
    { _id: studentId2 },
    { first_name: 1, last_name: 1, course_ratings: 1 }
);
console.log(JSON.stringify(studentWithNewRating, null, 2));

// ========== DISADVANTAGES OF VARIANT C ==========

// Query 5: Get all grades for a specific student (disadvantage: requires unwinding)
console.log("\n========== DISADVANTAGE 1: All Grades for a Student (Requires $unwind All Courses) ==========");
const query5Result = db.courses.aggregate([
    { $unwind: "$grades" },
    { $match: { "grades.student_id": studentId1 } },
    {
        $project: {
            _id: 0,
            course_name: "$name",
            student_grade: "$grades.grade",
            grade_date: "$grades.date"
        }
    }
]).toArray();
console.log(JSON.stringify(query5Result, null, 2));

// Query 6: Update a single grade using array filters (disadvantage: complex)
console.log("\n========== DISADVANTAGE 2: Updating a Grade (Array Filters Required) ==========");
const updateResult = db.courses.updateOne(
    { _id: courseId1, "grades.student_id": studentId1 },
    { $set: { "grades.$[elem].grade": 4.7 } },
    { arrayFilters: [{ "elem.student_id": studentId1 }] }
);
console.log("Update Result (requires arrayFilters):");
console.log(JSON.stringify(updateResult, null, 2));

// Verify the update
console.log("\nVerification - Updated Course:");
const updatedCourse = db.courses.findOne({ _id: courseId1 });
console.log(JSON.stringify(updatedCourse, null, 2));

// Query 7: Course average grade (disadvantage: requires $unwind)
console.log("\n========== DISADVANTAGE 3: Course Average Grade (Requires $unwind) ==========");
const query7Result = db.courses.aggregate([
    { $match: { _id: courseId1 } },
    { $unwind: "$grades" },
    {
        $group: {
            _id: "$_id",
            course_name: { $first: "$name" },
            average_grade: { $avg: "$grades.grade" },
            total_students: { $sum: 1 }
        }
    }
]).toArray();
console.log(JSON.stringify(query7Result, null, 2));

// Query 8: Enroll student to course (disadvantage: requires 2 updates + document grows)
console.log("\n========== DISADVANTAGE 4: Enroll Student to Course (2 Updates + Document Grows) ==========");
const enrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $push: { course_ids: courseId2 } }
);
console.log("Update 1 - Add course to student:");
console.log(JSON.stringify(enrollResult1, null, 2));

const enrollResult2 = db.courses.updateOne(
    { _id: courseId2 },
    { $push: { enrolled_students: { student_id: studentId2, student_number: "S12346" } } }
);
console.log("Update 2 - Add student to course (document size grows):");
console.log(JSON.stringify(enrollResult2, null, 2));

// Query 9: Unenroll student from course (disadvantage: requires 2 updates)
console.log("\n========== DISADVANTAGE 5: Unenroll Student from Course (2 Updates) ==========");
const unenrollResult1 = db.students.updateOne(
    { _id: studentId2 },
    { $pull: { course_ids: courseId1 } }
);
console.log("Update 1 - Remove course from student:");
console.log(JSON.stringify(unenrollResult1, null, 2));

const unenrollResult2 = db.courses.updateOne(
    { _id: courseId1 },
    { $pull: { enrolled_students: { student_id: studentId2 } } }
);
console.log("Update 2 - Remove student from course:");
console.log(JSON.stringify(unenrollResult2, null, 2));
```

Rezultat:
```json
========== ADVANTAGE 1: Course Card with All Grades (Single Document) ==========
{
  "_id": "6a00b9715908c65544519bac",
  "name": "Discrete Mathematics",
  "lecturer_id": "6a00b9715908c65544519ba9",
  "semester": 1,
  "credits": 5,
  "enrolled_students": [
    {
      "student_id": "6a00b9715908c65544519baa",
      "student_number": "S12345"
    },
    {
      "student_id": "6a00b9715908c65544519bab",
      "student_number": "S12346"
    }
  ],
  "grades": [
    {
      "student_id": "6a00b9715908c65544519baa",
      "grade": 4.5,
      "date": "2025-12-15T00:00:00.000Z"
    },
    {
      "student_id": "6a00b9715908c65544519bab",
      "grade": 3.8,
      "date": "2025-12-15T00:00:00.000Z"
    }
  ]
}

========== ADVANTAGE 2: Student Course Ratings (Nested in Student) ==========
[
  {
    "student_name": "Alice Johnson",
    "course_name": "Discrete Mathematics",
    "rating": 5,
    "comment": "Excellent course!",
    "date": "2025-12-20T00:00:00.000Z"
  },
  {
    "student_name": "Alice Johnson",
    "course_name": "Data Structures",
    "rating": 4,
    "comment": "Good, but challenging",
    "date": "2025-12-21T00:00:00.000Z"
  }
]

========== ADVANTAGE 3: Adding a New Grade to Course (Simple Push) ==========
Add Grade Result (single operation):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

Verification - Updated Grades in Course:
{
  "_id": "6a00b9715908c65544519bad",
  "name": "Data Structures",
  "grades": [
    {
      "student_id": "6a00b9715908c65544519baa",
      "grade": 4.2,
      "date": "2025-12-16T00:00:00.000Z"
    },
    {
      "student_id": "6a00b9715908c65544519bab",
      "grade": 3.9,
      "date": "2025-12-16T00:00:00.000Z"
    }
  ]
}

========== ADVANTAGE 4: Adding a New Course Rating to Student (Simple Push) ==========
Add Rating Result (single operation):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

Verification - Updated Student with New Rating:
{
  "_id": "6a00b9715908c65544519bab",
  "first_name": "Bob",
  "last_name": "Williams",
  "course_ratings": [
    {
      "course_id": "6a00b9715908c65544519bac",
      "rating": 4,
      "comment": "Good content",
      "date": "2025-12-20T00:00:00.000Z"
    },
    {
      "course_id": "6a00b9715908c65544519bad",
      "rating": 3,
      "comment": "Interesting but difficult",
      "date": "2025-12-22T00:00:00.000Z"
    }
  ]
}

========== DISADVANTAGE 1: All Grades for a Student (Requires $unwind All Courses) ==========
[
  {
    "course_name": "Discrete Mathematics",
    "student_grade": 4.5,
    "grade_date": "2025-12-15T00:00:00.000Z"
  },
  {
    "course_name": "Data Structures",
    "student_grade": 4.2,
    "grade_date": "2025-12-16T00:00:00.000Z"
  }
]

========== DISADVANTAGE 2: Updating a Grade (Array Filters Required) ==========
Update Result (requires arrayFilters):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

Verification - Updated Course:
{
  "_id": "6a00b9715908c65544519bac",
  "name": "Discrete Mathematics",
  "lecturer_id": "6a00b9715908c65544519ba9",
  "semester": 1,
  "credits": 5,
  "enrolled_students": [
    {
      "student_id": "6a00b9715908c65544519baa",
      "student_number": "S12345"
    },
    {
      "student_id": "6a00b9715908c65544519bab",
      "student_number": "S12346"
    }
  ],
  "grades": [
    {
      "student_id": "6a00b9715908c65544519baa",
      "grade": 4.7,
      "date": "2025-12-15T00:00:00.000Z"
    },
    {
      "student_id": "6a00b9715908c65544519bab",
      "grade": 3.8,
      "date": "2025-12-15T00:00:00.000Z"
    }
  ]
}

========== DISADVANTAGE 3: Course Average Grade (Requires $unwind) ==========
[
  {
    "_id": "6a00b9715908c65544519bac",
    "course_name": "Discrete Mathematics",
    "average_grade": 4.25,
    "total_students": 2
  }
]

========== DISADVANTAGE 4: Enroll Student to Course (2 Updates + Document Grows) ==========
Update 1 - Add course to student:
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}
Update 2 - Add student to course (document size grows):
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

========== DISADVANTAGE 5: Unenroll Student from Course (2 Updates) ==========
Update 1 - Remove course from student:
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}
Update 2 - Remove student from course:
{
  "acknowledged": true,
  "insertedId": null,
  "matchedCount": 1,
  "modifiedCount": 1,
  "upsertedCount": 0
}

```