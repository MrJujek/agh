import random
from datetime import date, timedelta
from faker import Faker

# Ustawienia
LOCALE = 'pl_PL'
OUTPUT_FILE = 'insert_data.sql'
fake = Faker(LOCALE)

# --- KONFIGURACJA LICZBOWA (ZGODNA Z WYMAGANIAMI) ---
COUNTS = {
    'countries': 3,
    'regions': 5,
    'cities': 8,           # Mniej miast, aby dane się powtarzały
    'suppliers': 5,
    'shippers': 3,
    'categories': 5,
    'parts': 30,
    'products': 15,        # <--- KILKANAŚCIE PRODUKTÓW
    'labor_types': 4,
    'vat': 3,
    'storage': 2,
    'employees': 5,        # Mały zespół
    'customers': 15,       # <--- KILKUNASTU KLIENTÓW
    'orders': 100,         # <--- OKOŁO 100 ZAMÓWIEŃ
    'part_orders': 15,
    'production_plan': 15
}

def escape_str(s):
    # Podwajamy apostrofy, aby SQL był poprawny (np. O'Connor -> O''Connor)
    return f"'{str(s).replace("'", "''")}'"

def date_to_str(d):
    return f"'{d.strftime('%Y-%m-%d')}'"

def generate_sql():
    f = open(OUTPUT_FILE, 'w', encoding='utf-8')
    
    print(f"Generowanie danych do pliku {OUTPUT_FILE}...")
    
    # 1. Tabela: Countries
    print(" - Countries")
    country_ids = []
    for i in range(1, COUNTS['countries'] + 1):
        name = fake.country()
        f.write(f"INSERT INTO Countries (CountryID, Name) VALUES ({i}, {escape_str(name)});\n")
        country_ids.append(i)

    # 2. Tabela: Regions
    print(" - Regions")
    region_ids = []
    for i in range(1, COUNTS['regions'] + 1):
        name = fake.voivodeship()
        country_id = random.choice(country_ids)
        f.write(f"INSERT INTO Regions (RegionID, Name, CountryID) VALUES ({i}, {escape_str(name)}, {country_id});\n")
        region_ids.append(i)

    # 3. Tabela: Cities
    print(" - Cities")
    city_ids = []
    for i in range(1, COUNTS['cities'] + 1):
        name = fake.city()
        region_id = random.choice(region_ids)
        f.write(f"INSERT INTO Cities (CityID, Name, RegionID) VALUES ({i}, {escape_str(name)}, {region_id});\n")
        city_ids.append(i)

    # 4. Tabela: VAT
    print(" - VAT")
    vat_ids = []
    vat_rates = [0, 8, 23]
    for i, rate in enumerate(vat_rates, 1):
        f.write(f"INSERT INTO VAT (VatID, Amount) VALUES ({i}, {rate});\n")
        vat_ids.append(i)

    # 5. PartsCategories & ProductsCategories
    print(" - Categories")
    part_cat_ids = []
    prod_cat_ids = []
    categories = ['Elektronika', 'Metal', 'Plastik', 'Drewno', 'Chemia']
    
    for i, cat in enumerate(categories, 1):
        f.write(f"INSERT INTO PartsCategories (CategoryID, CategoryName) VALUES ({i}, {escape_str(cat)});\n")
        f.write(f"INSERT INTO ProductsCategories (CategoryID, Name) VALUES ({i}, {escape_str('Wyroby: ' + cat)});\n")
        part_cat_ids.append(i)
        prod_cat_ids.append(i)

    # 6. CategoryVat
    print(" - CategoryVat")
    for cat_id in prod_cat_ids:
        vat_id = random.choice(vat_ids)
        d_from = date_to_str(fake.date_between(start_date='-5y', end_date='-3y'))
        f.write(f'INSERT INTO CategoryVat (CategoryID, VatID, "From", "To") VALUES ({cat_id}, {vat_id}, {d_from}, NULL);\n')

    # 7. LaborTypes
    print(" - LaborTypes")
    labor_ids = []
    labors = [('Spawanie', 120), ('Montaż ręczny', 60), ('CNC', 200), ('Pakowanie', 40)]
    for i, (name, rate) in enumerate(labors, 1):
        f.write(f"INSERT INTO LaborTypes (LaborTypeID, LaborName, HourlyRate) VALUES ({i}, {escape_str(name)}, {rate});\n")
        labor_ids.append(i)

    # 8. OrderStatus & ProductionStatus
    print(" - Statuses")
    status_ids = []
    # Skoro to historyczne zamówienia, większość będzie zakończona
    statuses = ['Nowe', 'W trakcie', 'Wysłane', 'Zakończone', 'Anulowane']
    for i, s in enumerate(statuses, 1):
        f.write(f"INSERT INTO OrderStatus (StatusID, Name) VALUES ({i}, {escape_str(s)});\n")
        status_ids.append(i)
        
    prod_status_ids = []
    prod_statuses = ['Planowane', 'W produkcji', 'Gotowe', 'Wstrzymane']
    for i, s in enumerate(prod_statuses, 1):
        f.write(f"INSERT INTO ProductionStatus (StatusID, StatusName) VALUES ({i}, {escape_str(s)});\n")
        prod_status_ids.append(i)

    # 9. Shippers & Suppliers
    print(" - Shippers & Suppliers")
    shipper_ids = []
    for i in range(1, COUNTS['shippers'] + 1):
        f.write(f"INSERT INTO Shippers (ShipperID, Name) VALUES ({i}, {escape_str(fake.company() + ' Logistics')});\n")
        shipper_ids.append(i)

    supplier_ids = []
    for i in range(1, COUNTS['suppliers'] + 1):
        f.write(f"INSERT INTO Suppliers (SupplierID, CompanyName, Phone) VALUES ({i}, {escape_str(fake.company())}, {escape_str(str(fake.phone_number())[:13])});\n")
        supplier_ids.append(i)

    # 10. Storage
    print(" - Storage")
    storage_ids = []
    for i in range(1, COUNTS['storage'] + 1):
        city_id = random.choice(city_ids)
        f.write(f"INSERT INTO Storage (StorageID, CityID, Address) VALUES ({i}, {city_id}, {escape_str(fake.street_address())});\n")
        storage_ids.append(i)

    # 11. Parts
    print(" - Parts")
    part_ids = []
    for i in range(1, COUNTS['parts'] + 1):
        cat_id = random.choice(part_cat_ids)
        name = f"Element {fake.word().upper()} {random.randint(10,99)}"
        f.write(f"INSERT INTO Parts (PartID, CategoryID, Name) VALUES ({i}, {cat_id}, {escape_str(name)});\n")
        part_ids.append(i)

    # 12. Products (Kilkanaście)
    print(" - Products")
    product_ids = []
    product_names_base = ['Krzesło', 'Stół', 'Lampa', 'Biurko', 'Szafa', 'Fotel', 'Rama', 'Silnik', 'Pompa', 'Sterownik', 'Obudowa', 'Płyta', 'Zawór', 'Czujnik', 'Panel']
    # Bierzemy tyle nazw, ile zdefiniowano w COUNTS, lub powtarzamy jeśli trzeba
    for i in range(1, COUNTS['products'] + 1):
        cat_id = random.choice(prod_cat_ids)
        base_name = product_names_base[i % len(product_names_base)]
        full_name = f"{base_name} {fake.word().capitalize()} {random.randint(100, 500)}"
        
        price = round(random.uniform(100.0, 3000.0), 2)
        prod_cost = round(price * 0.55, 2)
        time = random.randint(30, 480)
        
        f.write(f"INSERT INTO Products (ProductID, UnitPrice, CategoryID, Name, ProductionTime, ProductionCost) VALUES ({i}, {price}, {cat_id}, {escape_str(full_name)}, {time}, {prod_cost});\n")
        product_ids.append(i)

    # 13. WhatSupply
    print(" - WhatSupply")
    what_supply_pairs = []
    for part in part_ids:
        chosen_suppliers = random.sample(supplier_ids, k=random.randint(1, 2))
        for supp in chosen_suppliers:
            price = round(random.uniform(5.0, 500.0), 2)
            time = random.randint(1, 7)
            f.write(f"INSERT INTO WhatSupply (PartID, SupplierID, Price, DeliveryTime) VALUES ({part}, {supp}, {price}, {time});\n")
            what_supply_pairs.append((part, supp))

    # 14. Employees
    print(" - Employees")
    employee_ids = []
    boss_id = 1
    city_id = random.choice(city_ids)
    f.write(f"INSERT INTO Employees (EmployeeID, ReportsTo, CityID, LastName, FirstName, Title, TitleOfCourtesy, BirthDate, HireDate, HomePhone, Notes, Address) "
            f"VALUES ({boss_id}, NULL, {city_id}, {escape_str(fake.last_name())}, {escape_str(fake.first_name())}, 'Kierownik', 'Pan/Pani', "
            f"{date_to_str(fake.date_of_birth(minimum_age=45, maximum_age=65))}, {date_to_str(fake.date_between(start_date='-15y', end_date='-10y'))}, "
            f"{escape_str(fake.phone_number())}, 'Szef', {escape_str(fake.street_address())});\n")
    employee_ids.append(boss_id)

    for i in range(2, COUNTS['employees'] + 1):
        city_id = random.choice(city_ids)
        f.write(f"INSERT INTO Employees (EmployeeID, ReportsTo, CityID, LastName, FirstName, Title, TitleOfCourtesy, BirthDate, HireDate, HomePhone, Notes, Address) "
                f"VALUES ({i}, {boss_id}, {city_id}, {escape_str(fake.last_name())}, {escape_str(fake.first_name())}, 'Pracownik', 'Pan/Pani', "
                f"{date_to_str(fake.date_of_birth(minimum_age=20, maximum_age=40))}, {date_to_str(fake.date_between(start_date='-5y', end_date='-1y'))}, "
                f"{escape_str(fake.phone_number())}, '', {escape_str(fake.street_address())});\n")
        employee_ids.append(i)

    # 15. Customers (Kilkunastu)
    print(" - Customers")
    customer_ids = []
    for i in range(1, COUNTS['customers'] + 1):
        city_id = random.choice(city_ids)
        phone = str(fake.phone_number()).replace(' ', '')[:13]
        email = fake.unique.email()
        addr = fake.street_address()
        f.write(f"INSERT INTO Customers (CustomerID, Phone, Email, CityID, Address) VALUES ({i}, {escape_str(phone)}, {escape_str(email)}, {city_id}, {escape_str(addr)});\n")
        customer_ids.append(i)
        
        # Mieszamy firmy i osoby
        if random.random() > 0.4:
            nip = fake.unique.nip()
            comp_name = fake.company()
            f.write(f"INSERT INTO Companies (CustomerID, CompanyName, NIP) VALUES ({i}, {escape_str(comp_name)}, {escape_str(nip)});\n")
        else:
            f.write(f"INSERT INTO Individuals (CustomerID, Firstname, Lastname) VALUES ({i}, {escape_str(fake.first_name())}, {escape_str(fake.last_name())});\n")

    # 16. Stocks (Parts & Products)
    print(" - Storage Stocks")
    for store in storage_ids:
        for part in part_ids:
            if random.random() > 0.3:
                f.write(f"INSERT INTO PartsStorage (StorageID, PartID, Quantity) VALUES ({store}, {part}, {random.randint(0, 500)});\n")
        for prod in product_ids:
            if random.random() > 0.3:
                f.write(f"INSERT INTO ProductStorage (StorageID, ProductID, Quantity) VALUES ({store}, {prod}, {random.randint(0, 100)});\n")

    # 17. Recipes & Labor
    print(" - Recipes & Labor")
    for prod in product_ids:
        parts_needed = random.sample(part_ids, k=random.randint(2, 6))
        for p in parts_needed:
            f.write(f"INSERT INTO Recipes (ProductID, PartID, Count) VALUES ({prod}, {p}, {random.randint(1, 8)});\n")
        
        labors_needed = random.sample(labor_ids, k=random.randint(1, 2))
        for l in labors_needed:
            # Uproszczone pobieranie stawki
            rate = 50
            for lid, lname, lrate in [(xx, n, r) for xx, (n, r) in enumerate(labors, 1)]:
                if lid == l: rate = lrate
            f.write(f"INSERT INTO ProductLabor (ProductID, LaborTypeID, MinutesOfWork, HourlyRate) VALUES ({prod}, {l}, {random.randint(20, 180)}, {rate});\n")

    # 18. Orders (Około 100 historycznych)
    print(" - Orders")
    order_ids = []
    for i in range(1, COUNTS['orders'] + 1):
        cust_id = random.choice(customer_ids)
        emp_id = random.choice(employee_ids)
        ship_id = random.choice(shipper_ids)
        
        # Daty historyczne (do 2 lat wstecz)
        o_date = fake.date_between(start_date='-2y', end_date='-1m')
        req_date = o_date + timedelta(days=14)
        
        # Większość zamówień zakończona/wysłana
        if random.random() > 0.1:
            stat_id = 4 # Zakończone
            ship_date = o_date + timedelta(days=random.randint(1, 5))
            shipped_val = date_to_str(ship_date)
        else:
            stat_id = 5 # Anulowane
            shipped_val = date_to_str(o_date + timedelta(days=2)) # Wstawiamy cokolwiek, byle nie NULL jeśli logika wymaga, lub dummy date

        freight = round(random.uniform(15.0, 150.0), 2)
        
        f.write(f"INSERT INTO Orders (OrderID, CustomerID, EmployeeID, StatusID, ShipperID, OrderDate, RequiredDate, ShippedDate, Freight) "
                f"VALUES ({i}, {cust_id}, {emp_id}, {stat_id}, {ship_id}, {date_to_str(o_date)}, {date_to_str(req_date)}, {shipped_val}, {freight});\n")
        order_ids.append(i)

        # Details (każde zamówienie ma 1-4 pozycje)
        prods_in_order = random.sample(product_ids, k=random.randint(1, 4))
        for prod in prods_in_order:
            qty = random.randint(1, 20)
            discount = 0.0
            if qty > 10: discount = 0.1
            u_price = round(random.uniform(100, 1000), 2) # Symulacja ceny z momentu zakupu
            f.write(f"INSERT INTO OrderDetails (OrderID, ProductID, Quantity, Discount, UnitPrice) VALUES ({i}, {prod}, {qty}, {discount}, {u_price});\n")

    # 19. PartOrders
    print(" - PartOrders")
    for i in range(1, COUNTS['part_orders'] + 1):
        o_date = fake.date_between(start_date='-1y', end_date='-1m')
        ship_date = o_date + timedelta(days=2)
        rec_date = ship_date + timedelta(days=3)
        
        f.write(f"INSERT INTO PartOrders (OrderID, OrderDate, ShippedDate, ReceivedDate, EstimatedArrival, Freight) "
                f"VALUES ({i}, {date_to_str(o_date)}, {date_to_str(ship_date)}, {date_to_str(rec_date)}, {date_to_str(rec_date)}, {round(random.uniform(50,300),2)});\n")

        chosen_pairs = random.sample(what_supply_pairs, k=random.randint(1, 3))
        for (pid, sid) in chosen_pairs:
            f.write(f"INSERT INTO PartOrderDetails (OrderID, PartID, SupplierID, UnitPrice, Quantity) VALUES ({i}, {pid}, {sid}, {round(random.uniform(10,100),2)}, {random.randint(50, 200)});\n")

    # 20. ProductionPlan
    print(" - ProductionPlan")
    for i in range(1, COUNTS['production_plan'] + 1):
        prod = random.choice(product_ids)
        o_date = fake.date_between(start_date='-1y', end_date='-1m')
        s_date = o_date + timedelta(days=1)
        e_date = s_date + timedelta(days=5)
        
        f.write(f"INSERT INTO ProductionPlan (PlanID, ProductID, OrderDate, StartDate, EndDate, StatusID) "
                f"VALUES ({i}, {prod}, {date_to_str(o_date)}, {date_to_str(s_date)}, {date_to_str(e_date)}, 3);\n")

    f.close()
    print("Gotowe! Dane wygenerowane w insert_data.sql")

if __name__ == '__main__':
    generate_sql()