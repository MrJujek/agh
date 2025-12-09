import express from "express";
import { Request, Response, NextFunction } from "express";
import { AppDataSource } from "./data-source";
import { Book } from "./entity/Book";
import jwt from "jsonwebtoken";
import cors from "cors";
import dotenv from "dotenv";

dotenv.config();

export const app = express();
const JWT_SECRET = process.env.JWT_SECRET || 'supersecretkey';

app.use(cors());
app.use(express.json());

app.use((req: Request, res: Response, next: NextFunction) => {
    console.log(`[Books Service] ${req.method} ${req.url}`);
    next();
});

const authenticateToken = (req: Request, res: Response, next: NextFunction) => {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1];

    if (!token) {
        res.sendStatus(401);
        return;
    }

    jwt.verify(token, JWT_SECRET, (err: any, user: any) => {
        if (err) {
            res.sendStatus(403);
            return;
        }
        (req as any).user = user;
        next();
    });
};

app.get('/api/books', async (req: Request, res: Response) => {
    try {
        const bookRepository = AppDataSource.getRepository(Book);
        const books = await bookRepository.find();
        res.json(books);
    } catch (error: any) {
        res.status(500).json({ error: error.message });
    }
});

app.get('/api/books/:bookId', async (req: Request, res: Response): Promise<any> => {
    try {
        const bookRepository = AppDataSource.getRepository(Book);
        const book = await bookRepository.findOneBy({ id: parseInt(req.params.bookId) });
        if (!book) return res.status(404).json({ error: 'Book not found' });
        return res.json(book);
    } catch (error: any) {
        return res.status(500).json({ error: error.message });
    }
});

app.post('/api/books', authenticateToken, async (req: Request, res: Response) => {
    try {
        const { title, author, year } = req.body;
        const book = new Book();
        book.title = title;
        book.author = author;
        book.year = year;
        
        const bookRepository = AppDataSource.getRepository(Book);
        await bookRepository.save(book);
        res.status(201).json({ id: book.id });
    } catch (error: any) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/books/:bookId', authenticateToken, async (req: Request, res: Response): Promise<any> => {
    try {
        const bookRepository = AppDataSource.getRepository(Book);
        const book = await bookRepository.findOneBy({ id: parseInt(req.params.bookId) });
        if (!book) return res.status(404).json({ error: 'Book not found' });
        await bookRepository.remove(book);
        return res.json({ message: 'Book deleted' });
    } catch (error: any) {
        return res.status(500).json({ error: error.message });
    }
});
