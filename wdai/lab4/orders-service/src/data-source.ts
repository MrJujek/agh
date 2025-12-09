import "reflect-metadata"
import { DataSource } from "typeorm"
import { Order } from "./entity/Order"

export const AppDataSource = new DataSource({
    type: "sqlite",
    database: "database.sqlite",
    synchronize: true,
    logging: false,
    entities: [Order],
    migrations: [],
    subscribers: [],
})
