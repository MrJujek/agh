import { describe, it, expect, beforeAll, afterAll, mock } from "bun:test";
import request from "supertest";
import { app } from "../app";
import { AppDataSource } from "../data-source";
import { Order } from "../entity/Order";
import jwt from "jsonwebtoken";

const JWT_SECRET = process.env.JWT_SECRET || 'supersecretkey';

// Mock axios to avoid calling real Books Service
mock.module("axios", () => ({
    default: {
        get: mock(() => Promise.resolve({ status: 200, data: { id: 1, title: "Book" } }))
    }
}));

describe("Orders Service", () => {
    let token: string;

    beforeAll(async () => {
        if (!AppDataSource.isInitialized) {
            await AppDataSource.initialize();
        }
        token = jwt.sign({ id: 1, email: "test@example.com" }, JWT_SECRET);
    });

    afterAll(async () => {
        const orderRepository = AppDataSource.getRepository(Order);
        await orderRepository.clear();
    });

    it("should create an order", async () => {
        const res = await request(app)
            .post("/api/orders")
            .set("Authorization", `Bearer ${token}`)
            .send({
                userId: 1,
                bookId: 1,
                quantity: 1
            });
        
        expect(res.status).toBe(201);
        expect(res.body).toHaveProperty("id");
    });

    it("should get orders for user", async () => {
        // Create an order first
        await request(app)
            .post("/api/orders")
            .set("Authorization", `Bearer ${token}`)
            .send({
                userId: 1,
                bookId: 1,
                quantity: 1
            });

        const res = await request(app).get("/api/orders/1");
        expect(res.status).toBe(200);
        expect(Array.isArray(res.body)).toBe(true);
        expect(res.body.length).toBeGreaterThan(0);
    });

    it("should update an order", async () => {
        // Create an order first
        const createRes = await request(app)
            .post("/api/orders")
            .set("Authorization", `Bearer ${token}`)
            .send({
                userId: 1,
                bookId: 1,
                quantity: 1
            });
        const orderId = createRes.body.id;

        const res = await request(app)
            .patch(`/api/orders/${orderId}`)
            .set("Authorization", `Bearer ${token}`)
            .send({
                quantity: 5
            });
        
        expect(res.status).toBe(200);
        expect(res.body).toHaveProperty("quantity", 5);
    });

    it("should delete an order", async () => {
        // Create an order first
        const createRes = await request(app)
            .post("/api/orders")
            .set("Authorization", `Bearer ${token}`)
            .send({
                userId: 1,
                bookId: 1,
                quantity: 1
            });
        const orderId = createRes.body.id;

        const res = await request(app)
            .delete(`/api/orders/${orderId}`)
            .set("Authorization", `Bearer ${token}`);
        
        expect(res.status).toBe(200);
        expect(res.body).toHaveProperty("message", "Order deleted");
    });
});
