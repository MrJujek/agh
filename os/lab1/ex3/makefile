CC = gcc
RM = rm -f
CFLAGS = -Wall -O3
LDFLAGS = -lm

SRC_DIR = src
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECS = $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%)

.PHONY: all clean

all: $(EXECS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_DIR)/%: $(SRC_DIR)/%.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	-@ $(RM) $(OBJECTS) 
	-@ rm -rf $(BIN_DIR)