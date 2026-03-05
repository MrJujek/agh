use Library;

-- 1
SELECT
    m.firstname AS Imie,
    m.lastname AS Nazwisko,
    COUNT(j.member_no) AS Ilosc_Dzieci
FROM adult AS a
    JOIN juvenile AS j ON a.member_no = j.adult_member_no
    JOIN member AS m ON a.member_no = m.member_no
WHERE a.state = 'AZ'
GROUP BY a.member_no, m.firstname, m.lastname
HAVING COUNT(j.member_no) > 2

-- 2
SELECT
    m.firstname AS Imie,
    m.lastname AS Nazwisko,
    a.state AS Stan,
    COUNT(j.member_no) AS Ilosc_Dzieci
FROM adult AS a
    JOIN juvenile AS j ON a.member_no = j.adult_member_no
    JOIN member AS m ON a.member_no = m.member_no
WHERE a.state IN ('AZ', 'CA')
GROUP BY a.member_no, m.firstname, m.lastname, a.state
HAVING (a.state = 'AZ' AND COUNT(j.member_no) > 2) OR (a.state = 'CA' AND COUNT(j.member_no) > 3)