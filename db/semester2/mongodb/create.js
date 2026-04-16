const { execSync } = require("child_process");

const DB = "mydb";
const DUMP = "./dump/";

console.log("Dropping database...");
execSync(`mongosh --eval "db.getSiblingDB('${DB}').dropDatabase()"`, { stdio: "inherit" });

console.log("Restoring dump...");
execSync(`mongorestore --db ${DB} ${DUMP}`, { stdio: "inherit" });

console.log("Done!");