import { describe, it, expect, beforeAll, afterAll } from "bun:test";
import request from "supertest";
import { app } from "../app";
import { AppDataSource } from "../data-source";
import { User } from "../entity/User";

describe("Users Service", () => {
    beforeAll(async () => {
        if (!AppDataSource.isInitialized) {
            await AppDataSource.initialize();
        }
    });

    afterAll(async () => {
        const userRepository = AppDataSource.getRepository(User);
        await userRepository.clear();
    });

    it("should register a new user", async () => {
        const res = await request(app)
            .post("/api/register")
            .send({
                email: "test@example.com",
                password: "password123"
            });
        
        expect(res.status).toBe(201);
        expect(res.body).toHaveProperty("id");
    });

    it("should login with registered user", async () => {
        const res = await request(app)
            .post("/api/login")
            .send({
                email: "test@example.com",
                password: "password123"
            });
        
        expect(res.status).toBe(200);
        expect(res.body).toHaveProperty("token");
    });

    it("should fail login with wrong password", async () => {
        const res = await request(app)
            .post("/api/login")
            .send({
                email: "test@example.com",
                password: "wrongpassword"
            });
        
        expect(res.status).toBe(401);
    });
});
