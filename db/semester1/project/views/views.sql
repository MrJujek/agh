CREATE VIEW View_RaportPlanuProdukcji
AS
    SELECT
        pp.PlanID,
        p.Name AS NazwaProduktu,
        ps.StatusName AS StatusProdukcji,
        pp.OrderDate AS DataZlecenia,
        pp.StartDate AS DataRozpoczecia,
        pp.EndDate AS DataZakonczenia,
        DATEDIFF(day, pp.StartDate, pp.EndDate) AS PlanowanyCzasDni
    FROM
        ProductionPlan pp
        JOIN
        Products p ON pp.ProductID = p.ProductID
        JOIN
        ProductionStatus ps ON pp.StatusID = ps.StatusID;

SELECT *
FROM View_RaportPlanuProdukcji
WHERE DataRozpoczecia >= '2026-01-01' AND DataRozpoczecia <= '2026-12-31'
ORDER BY DataRozpoczecia;

SELECT NazwaProduktu, DataZakonczenia, StatusProdukcji
FROM View_RaportPlanuProdukcji
WHERE DataZakonczenia BETWEEN '2026-01-01' AND '2026-02-28';

SELECT
    YEAR(DataRozpoczecia) as Rok,
    MONTH(DataRozpoczecia) as Miesiac,
    COUNT(*) as IloscZlecen
FROM View_RaportPlanuProdukcji
GROUP BY YEAR(DataRozpoczecia), MONTH(DataRozpoczecia)
ORDER BY Rok, Miesiac;



CREATE VIEW View_RaportSprzedazyKategorii
AS
    SELECT
        DATEPART(year, o.OrderDate) AS Rok,
        DATEPART(month, o.OrderDate) AS Miesiac,
        DATEPART(week, o.OrderDate) AS Tydzien,
        pc.Name AS KategoriaProduktu,
        ROUND(SUM(od.UnitPrice * od.Quantity * (1 - od.Discount)), 2) AS WartoscSprzedazy,
        COUNT(DISTINCT o.OrderID) AS LiczbaZamowien
    FROM
        Orders o
        JOIN
        OrderDetails od ON o.OrderID = od.OrderID
        JOIN
        Products p ON od.ProductID = p.ProductID
        JOIN
        ProductsCategories pc ON p.CategoryID = pc.CategoryID
    GROUP BY
    DATEPART(year, o.OrderDate),
    DATEPART(month, o.OrderDate),
    DATEPART(week, o.OrderDate),
    pc.Name;

SELECT Rok, Miesiac, KategoriaProduktu, WartoscSprzedazy
FROM View_RaportSprzedazyKategorii
ORDER BY Rok, Miesiac, WartoscSprzedazy DESC;