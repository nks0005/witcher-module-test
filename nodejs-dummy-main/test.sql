-- test.sql
USE mysql;
ALTER USER 'root'@'localhost' IDENTIFIED BY 'password';

CREATE DATABASE IF NOT EXISTS dummy;
USE dummy;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL
);

INSERT INTO users (username, password) VALUES ('admin', 'admin');
INSERT INTO users (username, password) VALUES ('guest', 'guest');

ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'password';


ALTER USER 'youruser'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'yourpassword';

ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'password';

