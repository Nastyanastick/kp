CREATE DATABASE demo;
USE demo;

CREATE TABLE users (
id int INDEXED,
name string NOT_NULL,
age int,
city string,
role string
);

INSERT INTO users VALUES
(1, "Anna", 20, "Moscow", "admin"),
(2, "Ivan", 25, "Kazan", "user"),
(3, "Oleg", 30, "Moscow", "user"),
(4, "Maria", 19, "Sochi", "manager"),
(5, "Pavel", 35, "Moscow", "user"),
(6, "Alina", 28, "Kazan", "admin"),
(7, "Boris", 40, "Perm", "user");

SELECT * FROM users;

SELECT * FROM users WHERE id == 1;

SELECT * FROM users WHERE id BETWEEN 2 AND 6;

SELECT * FROM users
WHERE age >= 25 AND city == "Moscow";

SELECT * FROM users
WHERE 25 <= age AND city == "Moscow";

SELECT * FROM users
WHERE city == "Moscow" OR city == "Kazan";

SELECT * FROM users
WHERE (city == "Moscow" OR city == "Kazan") AND age >= 28;

SELECT * FROM users
WHERE (city == "Moscow" AND age >= 30) OR (city == "Kazan" AND role == "admin");

SELECT * FROM users
WHERE ((city == "Moscow" OR city == "Kazan") AND (age >= 25 OR role == "manager")) AND id >= 2;

SELECT * FROM users
WHERE ((city == "Moscow" AND age BETWEEN 20 AND 36) OR (city == "Kazan" AND name LIKE "A%")) AND role != "manager";

SELECT * FROM users
WHERE name LIKE "A%";

SELECT * FROM users
WHERE name LIKE "%a";

SELECT * FROM users
WHERE city >= "Kazan" AND city < "Perm";

SELECT name AS username, city AS user_city, role AS user_role
FROM users
WHERE (role == "admin" OR role == "manager") AND age < 30;

SELECT COUNT(*) AS total_users FROM users;

SELECT COUNT(age) AS known_age_count FROM users;

SELECT SUM(age) AS age_sum FROM users;

SELECT AVG(age) AS average_age FROM users;

SELECT COUNT(*) AS moscow_or_kazan_count
FROM users
WHERE city == "Moscow" OR city == "Kazan";

SELECT SUM(age) AS adult_sum
FROM users
WHERE age >= 25 AND role != "manager";

UPDATE users SET city = "SaintPetersburg" WHERE id == 2;

SELECT * FROM users WHERE id == 2;

UPDATE users SET role = "senior" WHERE (city == "Moscow" AND age >= 30) OR name == "Alina";

SELECT * FROM users WHERE role == "senior";

DELETE FROM users WHERE (role == "senior" AND age >= 30) OR city == "Sochi";

SELECT * FROM users;

INSERT INTO users VALUES (1, "Duplicate", 50, "Moscow", "user");

INSERT INTO users VALUES (8, NULL, 22, "Moscow", "user");

INSERT INTO users VALUES (9, "BadAge", "abc", "Moscow", "user");

SELECT unknown_column FROM users;

UPDATE users SET id = 1 WHERE name == "Boris";

SELECT * FROM users WHERE "Anna" == name;

DROP TABLE users;
DROP DATABASE demo;
