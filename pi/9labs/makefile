CC = gcc
CFLAGS = -std=c2x -pedantic -Werror=pedantic -Wall -Wextra

BIN_DIR = bin
SRC_DIR = src
LIB_DIR = lib
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = $(BIN_DIR)/program

all: $(TARGET) run
	
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run:
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
