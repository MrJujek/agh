# Microservices Bookstore

A robust microservices-based bookstore application built with **Bun**, **TypeORM**, and **TypeScript**. This project demonstrates a distributed architecture with independent services for managing books, orders, and users.

## 🚀 Services Overview

The application is composed of three distinct microservices, each running on its own port:

| Service | Port | Description |
| :--- | :--- | :--- |
| **Books Service** | `3001` | Manages the book inventory. |
| **Orders Service** | `3002` | Handles customer orders and interacts with the Books Service. |
| **Users Service** | `3003` | Manages user authentication (JWT) and registration. |

## 🛠️ Prerequisites

- **[Bun](https://bun.sh/)** (v1.0.0 or later recommended)

## 📦 Installation

To set up the project, install dependencies from the root directory:

```bash
bun install
```

## 🏃‍♂️ Running the Services

You can run all services simultaneously from the root directory:

```bash
bun start
```

Or run them individually:

### 1. Books Service
```bash
cd books-service
bun start
```

### 2. Orders Service
```bash
cd orders-service
bun start
```

### 3. Users Service
```bash
cd users-service
bun start
```

## 🧪 Running Tests

To run tests for all services:

```bash
bun test
```

## 📡 API Endpoints

### 👤 Users Service (`http://localhost:3003`)

| Method | Endpoint | Description | Body / Headers |
| :--- | :--- | :--- | :--- |
| `POST` | `/api/register` | Register a new user | `{ "email": "...", "password": "..." }` |
| `POST` | `/api/login` | Login and receive JWT | `{ "email": "...", "password": "..." }` |

### 📚 Books Service (`http://localhost:3001`)

| Method | Endpoint | Description | Body / Headers |
| :--- | :--- | :--- | :--- |
| `GET` | `/api/books` | List all books | - |
| `GET` | `/api/books/:id` | Get book details | - |
| `POST` | `/api/books` | Add a new book | **Auth Header**, `{ "title": "...", "author": "...", "year": ... }` |
| `DELETE` | `/api/books/:id` | Remove a book | **Auth Header** |

### 🛒 Orders Service (`http://localhost:3002`)

| Method | Endpoint | Description | Body / Headers |
| :--- | :--- | :--- | :--- |
| `GET` | `/api/orders/:userId` | Get orders for a user | - |
| `POST` | `/api/orders` | Create a new order | **Auth Header**, `{ "userId": ..., "bookId": ..., "quantity": ... }` |
| `DELETE` | `/api/orders/:id` | Cancel an order | **Auth Header** |
| `PATCH` | `/api/orders/:id` | Update order quantity | **Auth Header**, `{ "quantity": ... }` |

## 📂 Project Structure

```
.
├── books-service/      # Inventory management service
├── orders-service/     # Order processing service
├── users-service/      # Authentication and user management
├── .gitignore          # Git ignore rules
└── README.md           # Project documentation
```
