# Dokumentowe bazy danych – MongoDB

Ćwiczenie 2


---

**Imiona i nazwiska autorów:** Julian Dworzycki, Radosław Klamka

--- 

Odtwórz z backupu bazę `north0`

- najprostsza wersja

```
mongorestore dump
```

- to polecenie odtworzy wszystkie bazy danych znajdujące się we wskazanym folderze (w tym przypadku ` dump `) 
	- najłatwiej wgrać tam folder zawierający pliki z backupem i wykonać proste polecenie mongorestore 
- dokumentacja:
	- https://www.mongodb.com/docs/database-tools/mongorestore/

Wybierz bazę north0

Baza `north0` jest kopią relacyjnej bazy danych `Northwind`
- poszczególne kolekcje odpowiadają tabelom w oryginalnej bazie `Northwind`


# Zadanie 0 

zapoznaj się ze strukturą dokumentów w bazie `North0`

```js
db.customers.find()
db.orders.find();
db.orderdetails.find();

```

# Zadanie 1 - operacje wyszukiwania danych,  przetwarzanie dokumentów

# a)

stwórz kolekcję  `OrdersInfo`  zawierającą następujące dane o zamówieniach
- kolekcję  `OrdersInfo` należy stworzyć przekształcając dokumenty w oryginalnych kolekcjach `customers, orders, orderdetails, employees, shippers, products, categories, suppliers` do kolekcji  w której pojedynczy dokument opisuje jedno zamówienie

spodziewany wynik:

```js
[  
  {  
    "_id": ...
    
    OrderID": ... numer zamówienia
    
    "Customer": {  ... podstawowe informacje o kliencie skladającym  
      "CustomerID": ... identyfikator klienta
      "CompanyName": ... nazwa klienta
      "City": ... miasto 
      "Country": ... kraj 
    },  
    
    "Employee": {  ... podstawowe informacje o pracowniku obsługującym zamówienie
      "EmployeeID": ... idntyfikator pracownika 
      "FirstName": ... imie   
      "LastName": ... nazwisko
      "Title": ... stanowisko  
     
    },  
    
    "Dates": {
       "OrderDate": ... data złożenia zamówienia
       "RequiredDate": data wymaganej realizacji
    }

    "Orderdetails": [  ... pozycje/szczegóły zamówienia - tablica takich pozycji 
      {  
        "UnitPrice": ... cena
        "Quantity": ... liczba sprzedanych jednostek towaru
        "Discount": ... zniżka  
        "Value": ... wartośc pozycji zamówienia
        "product": { ... podstawowe informacje o produkcie 
          "ProductID": ... identyfikator produktu  
          "ProductName": ... nazwa produktu 
          "QuantityPerUnit": ... opis/opakowannie
          "CategoryID": ... identyfikator kategorii do której należy produkt
          "CategoryName" ... nazwę tej kategorii
        },  
      },  
      ...   
    ],  

    "Freight": ... opłata za przesyłkę
    "OrderTotal"  ... sumaryczna wartosc sprzedanych produktów

    "Shipment" : {  ... informacja o wysyłce
        "Shipper": { ... podstawowe inf o przewoźniku 
           "ShipperID":  
            "CompanyName":
        }  
        ... inf o odbiorcy przesyłki
        "ShipName": ...
        "ShipAddress": ...
        "ShipCity": ... 
        "ShipCountry": ...
    } 
  } 
]  
```
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
var a_check = db.OrdersInfo.find().toArray();
console.log("====== a_check ======");
console.log(a_check);
```


# b)

stwórz kolekcję  `CustomerInfo`  zawierającą następujące dane każdym kliencie
- pojedynczy dokument opisuje jednego klienta

spodziewany wynik:

```js
[  
  {  
    "_id": ...
    
    "CustomerID": ... identyfikator klienta
    "CompanyName": ... nazwa klienta
    "City": ... miasto 
    "Country": ... kraj 

	"Orders": [ ... tablica zamówień klienta o strukturze takiej jak w punkcie a) 
	                (oczywiście bez informacji o kliencie)
	  
	]

		  
]  
```

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
var b_check = db.CustomerInfo.find().toArray();
console.log("====== b_check ======");
console.log(b_check);
```

# c) 

Napisz polecenie/zapytanie: Dla każdego klienta pokaż wartość zakupionych przez niego produktów z kategorii 'Confections'  w 1997r
- Spróbuj napisać to zapytanie wykorzystując
	- oryginalne kolekcje (`customers, orders, orderdertails, products, categories`)
	- kolekcję `OrderInfo`
	- kolekcję `CustomerInfo`

- porównaj zapytania/polecenia/wyniki

```js
[  
  {  
    "_id": 
    
    "CustomerID": ... identyfikator klienta
    "CompanyName": ... nazwa klienta
	"ConfectionsSale97": ... wartość zakupionych przez niego produktów 
	                         z kategorii 'Confections'  w 1997r

  }		  
]  
```

```js
var c = db.customers.aggregate([
    { $lookup: { from: "orders", localField: "CustomerID", foreignField: "CustomerID", as: "order" } },
    { $unwind: "$order" },
    { $match: { "order.OrderDate": { $regex: "^1997" } } },  // Fixed regex
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
```

# d)

Napisz polecenie/zapytanie:  Dla każdego klienta poaje wartość sprzedaży z podziałem na lata i miesiące
Spróbuj napisać to zapytanie wykorzystując
	- oryginalne kolekcje (`customers, orders, orderdertails, products, categories`)
	- kolekcję `OrderInfo`
	- kolekcję `CustomerInfo`

- porównaj zapytania/polecenia/wyniki

```js
[  
  {  
    "_id": 
    
    "CustomerID": ... identyfikator klienta
    "CompanyName": ... nazwa klienta

	"Sale": [ ... tablica zawierająca inf o sprzedazy
	    {
            "Year":  ....
            "Month": ....
            "Total": ...	    
	    }
	    ...
	]
  }		  
]  
```

```js
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

# f)

Napisz polecenie które modyfikuje zamówienie dodane w pkt e)  zwiększając zniżkę  o 5% (dla każdej pozycji tego zamówienia) 

Napisz polecenie 
- aktualizując oryginalną kolekcję `orderdetails`
- aktualizując kolekcję `OrderInfo`
- aktualizując kolekcję `CustomerInfo`



UWAGA:
W raporcie należy zamieścić kod poleceń oraz uzyskany rezultat, np wynik  polecenia `db.kolekcka.fimd().limit(2)` lub jego fragment


## Zadanie 1  - rozwiązanie

> Wyniki: 
> 
> przykłady, kod, zrzuty ekranów, komentarz ...

a)

```js
--  ...
```

b)


```js
--  ...
```

....

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




