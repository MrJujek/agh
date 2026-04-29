CC = gcc
CFLAGS = -std=c2x -O3 -pedantic -Werror=pedantic -Wall -Wextra -D_GNU_SOURCE

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

SERVER_SRC = $(SRC_DIR)/server.c
CLIENT_SRC = $(SRC_DIR)/client.c

SERVER_OBJ = $(OBJ_DIR)/server.o
CLIENT_OBJ = $(OBJ_DIR)/client.o

SERVER = $(BIN_DIR)/server
CLIENT = $(BIN_DIR)/client

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(SERVER): $(SERVER_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(CLIENT_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/common.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
