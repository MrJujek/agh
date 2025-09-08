CC = gcc
CFLAGS = -std=c2x -pedantic -Werror=pedantic -Wall -Wextra

SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib
TEST_DIR = test

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/math_utils.c $(SRC_DIR)/primes.c
OBJS = $(SRCS:.c=.o)
LIB_OBJS = $(SRC_DIR)/utils.o $(SRC_DIR)/primes.o
LIBRARY = $(LIB_DIR)/libutils.a
TARGET = $(BIN_DIR)/my_program
TEST_TARGET = $(BIN_DIR)/test_runner

# Domyślna reguła
all: $(TARGET) run

# Kompilacja biblioteki statycznej
$(LIBRARY): $(LIB_OBJS)
	ar rcs $(LIBRARY) $(LIB_OBJS)

# Kompilacja głównego programu
$(TARGET): $(OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -L$(LIB_DIR) -lutils -lm

# Reguła kompilacji testów
$(TEST_TARGET): $(TEST_DIR)/tests.c $(SRC_DIR)/math_utils.c
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_DIR)/tests.c $(SRC_DIR)/math_utils.c

# Reguła uruchamiania testów
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Kompilacja plików .c do .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

# Czyszczenie plików tymczasowych
clean:
	rm -f $(OBJS) $(LIB_OBJS) $(TARGET) $(TEST_TARGET) $(LIBRARY)

