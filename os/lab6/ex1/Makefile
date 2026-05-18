CC = gcc
CFLAGS = -std=c2x -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -O2 -pedantic -Werror=pedantic -Wall -Wextra -pthread

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

COMMON_SRCS = $(SRC_DIR)/common.c
PROGRAM1_SRCS = $(SRC_DIR)/program1.c $(COMMON_SRCS)
PROGRAM2_SRCS = $(SRC_DIR)/program2.c $(COMMON_SRCS)
PROGRAM3_SRCS = $(SRC_DIR)/program3.c $(COMMON_SRCS)

PROGRAM1_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(PROGRAM1_SRCS))
PROGRAM2_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(PROGRAM2_SRCS))
PROGRAM3_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(PROGRAM3_SRCS))

TARGETS = $(BIN_DIR)/program1 $(BIN_DIR)/program2 $(BIN_DIR)/program3

.PHONY: all run run1 run2 run3 clean

all: $(TARGETS)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR)/program1: $(PROGRAM1_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(PROGRAM1_OBJS)

$(BIN_DIR)/program2: $(PROGRAM2_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(PROGRAM2_OBJS)

$(BIN_DIR)/program3: $(PROGRAM3_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(PROGRAM3_OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: run1

run1: $(BIN_DIR)/program1
	./$(BIN_DIR)/program1

run2: $(BIN_DIR)/program2
	./$(BIN_DIR)/program2

run3: $(BIN_DIR)/program3
	./$(BIN_DIR)/program3

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)