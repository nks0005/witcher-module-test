const express = require('express');
const mysql = require('mysql');
const cors = require('cors');
const { exec } = require('child_process');
const app = express();
const port = 3000;

app.use(express.json());
app.use(cors());
app.use(express.static('public'));

const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'password',
    database: 'dummy'
});

connection.connect((err) => {
    if (err) {
        console.error("MySQL connection error : " + err.stack);
        return;
    }

    console.log("MySQL connection success " + connection.threadId);
});

app.get('/', (req, res) => {
    res.sendFile(__dirname + "/public/index.html");
});

app.get('/cmdi', (req, res) => {
    res.sendFile(__dirname + "/public/cmdi.html");
});

app.post('/cmdi', (req, res) => {
    const command = req.body.command;

    console.log("POST");

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error("Error occurred : ", error);
            res.send("Error occurred : " + error.message);
            return;
        }
        if (stderr) {
            console.error("CLI error : ", stderr);
            res.send("CLI error : " + stderr);
            return;
        }

        res.send("Result : " + stdout);
        console.log(stdout);
    })
});

app.get('/login', (req, res) => {
    res.sendFile(__dirname + "/public/login.html");
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;
    const query = `SELECT * FROM users WHERE username = '${username}' AND password = '${password}'`;

    try {
        connection.query(query, (err, results) => {
            if (err) throw err;

            if (results.length > 0) {
                res.send("Login success");
            } else {
                res.send("Login fail");
            }
        });
    } catch (error) {
        console.error("Database error:", error);
        res.status(500).send("Internal Server Error");
    }
});

app.listen(port, () => {
    console.log("Server is listening on port 3000");
});
