import "reflect-metadata";
import { app } from "./app";
import { AppDataSource } from "./data-source";
import fs from "fs";
import path from "path";

const PORT = process.env.PORT2 || 3002;

const dbPath = path.join(__dirname, "../database.sqlite");
if (!fs.existsSync(dbPath)) {
    fs.writeFileSync(dbPath, "");
    console.log("Created new database file: database.sqlite");
}

AppDataSource.initialize().then(async () => {
    console.log("Orders database connected");
    app.listen(PORT, () => {
        console.log(`Orders service running on port ${PORT}`);
    });
}).catch(error => console.log(error));
