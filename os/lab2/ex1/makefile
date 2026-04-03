CC = gcc
CFLAGS = -std=c2x -pedantic -Werror=pedantic -Wall -Wextra

BIN_DIR = bin
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = $(BIN_DIR)/program

all: $(TARGET) run

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

run:
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)