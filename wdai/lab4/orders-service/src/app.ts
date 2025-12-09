import express from "express";
import { Request, Response, NextFunction } from "express";
import { AppDataSource } from "./data-source";
import { Order } from "./entity/Order";
import jwt from "jsonwebtoken";
import axios from "axios";
import cors from "cors";
import dotenv from "dotenv";

dotenv.config();

export const app = express();
const JWT_SECRET = process.env.JWT_SECRET || 'supersecretkey';
const BOOKS_SERVICE_URL = process.env.BOOKS_SERVICE_URL || 'http://localhost:3001';

app.use(cors());
app.use(express.json());

app.use((req: Request, res: Response, next: NextFunction) => {
    console.log(`[Orders Service] ${req.method} ${req.url}`);
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

app.get('/api/orders/:userId', async (req: Request, res: Response) => {
    try {
        const orderRepository = AppDataSource.getRepository(Order);
        const orders = await orderRepository.findBy({ userId: parseInt(req.params.userId) });
        res.json(orders);
    } catch (error: any) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/orders', authenticateToken, async (req: Request, res: Response): Promise<any> => {
    try {
        const { userId, bookId, quantity } = req.body;

        try {
            await axios.get(`${BOOKS_SERVICE_URL}/api/books/${bookId}`);
        } catch (error: any) {
            if (error.response && error.response.status === 404) {
                return res.status(404).json({ error: 'Book not found' });
            }
            return res.status(500).json({ error: 'Error communicating with Books Service' });
        }

        const order = new Order();
        order.userId = userId;
        order.bookId = bookId;
        order.quantity = quantity;

        const orderRepository = AppDataSource.getRepository(Order);
        await orderRepository.save(order);
        return res.status(201).json({ id: order.id });
    } catch (error: any) {
        return res.status(500).json({ error: error.message });
    }
});

app.delete('/api/orders/:orderId', authenticateToken, async (req: Request, res: Response): Promise<any> => {
    try {
        const orderRepository = AppDataSource.getRepository(Order);
        const order = await orderRepository.findOneBy({ id: parseInt(req.params.orderId) });
        if (!order) return res.status(404).json({ error: 'Order not found' });
        await orderRepository.remove(order);
        return res.json({ message: 'Order deleted' });
    } catch (error: any) {
        return res.status(500).json({ error: error.message });
    }
});

app.patch('/api/orders/:orderId', authenticateToken, async (req: Request, res: Response): Promise<any> => {
    try {
        const orderRepository = AppDataSource.getRepository(Order);
        const order = await orderRepository.findOneBy({ id: parseInt(req.params.orderId) });
        if (!order) return res.status(404).json({ error: 'Order not found' });

        const { quantity } = req.body;
        if (quantity) order.quantity = quantity;

        await orderRepository.save(order);
        return res.json(order);
    } catch (error: any) {
        return res.status(500).json({ error: error.message });
    }
});
