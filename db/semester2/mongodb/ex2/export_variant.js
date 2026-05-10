const { execFileSync } = require("child_process");
const fs = require("fs");
const path = require("path");

function exportVariant({ variantName, sourceFile }) {
    const baseDir = __dirname;
    const dbName = `ex2_${variantName}`;
    const dumpRootDir = path.join(baseDir, "dump");
    const dumpDir = path.join(dumpRootDir, dbName);
    const sourcePath = path.join(baseDir, sourceFile);

    fs.rmSync(dumpDir, { recursive: true, force: true });
    fs.mkdirSync(dumpRootDir, { recursive: true });

    console.log(`Running ${sourceFile} in database ${dbName}...`);
    execFileSync(
        "mongosh",
        ["--quiet", "--eval", `db = db.getSiblingDB('${dbName}')`, "--file", sourcePath],
        { stdio: "inherit" }
    );

    console.log(`Creating dump in ${dumpDir}...`);
    execFileSync("mongodump", ["--db", dbName, "--out", dumpRootDir], { stdio: "inherit" });

    console.log(`Done: ${dumpDir}`);
}

module.exports = exportVariant;