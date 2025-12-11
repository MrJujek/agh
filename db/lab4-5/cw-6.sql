use Library;

-- 1
SELECT m.firstname AS Imie, m.lastname AS Nazwisko, j.birth_date AS Data_Urodzenia, CONCAT(a.street, ', ', a.city, ', ', a.state, ' ', a.zip) AS Adres_Zamieszkania
FROM juvenile AS j
    JOIN member AS m ON j.member_no = m.member_no
    JOIN adult AS a ON j.adult_member_no = a.member_no

-- 2
SELECT m_child.firstname AS Imie_Dziecka,
    m_child.lastname AS Nazwisko_Dziecka,
    j.birth_date AS Data_Urodzenia_Dziecka,
    CONCAT(a_parent.street, ', ', a_parent.city, ', ', a_parent.state, ' ', a_parent.zip) AS Adres_Zamieszkania,
    m_parent.firstname AS Imie_Rodzica,
    m_parent.lastname AS Nazwisko_Rodzica
FROM juvenile AS j
    JOIN member AS m_child ON j.member_no = m_child.member_no
    JOIN adult AS a_parent ON j.adult_member_no = a_parent.member_no
    JOIN member AS m_parent ON a_parent.member_no = m_parent.member_no