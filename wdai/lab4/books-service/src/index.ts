import "reflect-metadata";
import { app } from "./app";
import { AppDataSource } from "./data-source";
import fs from "fs";
import path from "path";

const PORT = process.env.PORT1 || 3001;

const dbPath = path.join(__dirname, "../database.sqlite");
if (!fs.existsSync(dbPath)) {
    fs.writeFileSync(dbPath, "");
    console.log("Created new database file: database.sqlite");
}

AppDataSource.initialize().then(async () => {
    console.log("Books database connected");
    app.listen(PORT, () => {
        console.log(`Books service running on port ${PORT}`);
    });
}).catch(error => console.log(error));
