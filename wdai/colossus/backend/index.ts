import express from "express";
import type { Request, Response, NextFunction } from "express";
import cors from "cors";

export const app = express();

const PORT = 3000;

app.use(cors());
app.use(express.json());

app.use((req: Request, res: Response, next: NextFunction) => {
  console.log(`${req.method} ${req.url}`);
  next();
});

app.listen(PORT, () => {
  console.log(`App running on port ${PORT}`);
});
