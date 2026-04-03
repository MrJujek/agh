CC = gcc
CFLAGS = -std=c2x -pedantic -Werror=pedantic -Wall -Wextra

BIN_DIR = bin
SRC_DIR = src

TARGET_MAIN = $(BIN_DIR)/main
TARGET_CHILD = $(BIN_DIR)/child

all: $(TARGET_MAIN) $(TARGET_CHILD)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET_MAIN): $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_CHILD): $(SRC_DIR)/child.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET_MAIN) $(TARGET_CHILD)
