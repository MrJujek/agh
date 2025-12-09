import { describe, it, expect, beforeAll, afterAll } from "bun:test";
import request from "supertest";
import { app } from "../app";
import { AppDataSource } from "../data-source";
import { Book } from "../entity/Book";
import jwt from "jsonwebtoken";

const JWT_SECRET = process.env.JWT_SECRET || 'supersecretkey';

describe("Books Service", () => {
    let token: string;

    beforeAll(async () => {
        if (!AppDataSource.isInitialized) {
            await AppDataSource.initialize();
        }
        token = jwt.sign({ id: 1, email: "test@example.com" }, JWT_SECRET);
    });

    afterAll(async () => {
        const bookRepository = AppDataSource.getRepository(Book);
        await bookRepository.clear();
    });

    it("should create a book", async () => {
        const res = await request(app)
            .post("/api/books")
            .set("Authorization", `Bearer ${token}`)
            .send({
                title: "Test Book",
                author: "Test Author",
                year: 2023
            });
        
        expect(res.status).toBe(201);
        expect(res.body).toHaveProperty("id");
    });

    it("should get all books", async () => {
        const res = await request(app).get("/api/books");
        expect(res.status).toBe(200);
        expect(Array.isArray(res.body)).toBe(true);
        expect(res.body.length).toBeGreaterThan(0);
    });

    it("should get a book by id", async () => {
        // Create a book first
        const createRes = await request(app)
            .post("/api/books")
            .set("Authorization", `Bearer ${token}`)
            .send({
                title: "Book to Get",
                author: "Author",
                year: 2023
            });
        const bookId = createRes.body.id;

        const res = await request(app).get(`/api/books/${bookId}`);
        expect(res.status).toBe(200);
        expect(res.body).toHaveProperty("id", bookId);
        expect(res.body).toHaveProperty("title", "Book to Get");
    });

    it("should delete a book", async () => {
        // Create a book first
        const createRes = await request(app)
            .post("/api/books")
            .set("Authorization", `Bearer ${token}`)
            .send({
                title: "Book to Delete",
                author: "Author",
                year: 2023
            });
        const bookId = createRes.body.id;

        const res = await request(app)
            .delete(`/api/books/${bookId}`)
            .set("Authorization", `Bearer ${token}`);
        
        expect(res.status).toBe(200);
        expect(res.body).toHaveProperty("message", "Book deleted");

        // Verify it's gone
        const getRes = await request(app).get(`/api/books/${bookId}`);
        expect(getRes.status).toBe(404);
    });
});
