CC = gcc
CFLAGS = -std=c2x -pedantic -Werror=pedantic -Wall -Wextra -D_POSIX_C_SOURCE=200809L

BIN_DIR = bin
SRC_DIR = src

SRCS_LIB = $(SRC_DIR)/sig_default.c $(SRC_DIR)/sig_ignore.c $(SRC_DIR)/sig_mask.c $(SRC_DIR)/sig_handle.c
OBJS_LIB = $(SRCS_LIB:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

OBJS_LIB_SHARED = $(SRCS_LIB:$(SRC_DIR)/%.c=$(BIN_DIR)/%_shared.o)

LIB_STATIC = $(BIN_DIR)/libhandlers.a
LIB_SHARED = $(BIN_DIR)/libhandlers.so

TARGET_STATIC = $(BIN_DIR)/main_static
TARGET_SHARED = $(BIN_DIR)/main_shared
TARGET_DYNAMIC = $(BIN_DIR)/main_dynamic

CHILD_STATIC = $(BIN_DIR)/child_static
CHILD_SHARED = $(BIN_DIR)/child_shared
CHILD_DYNAMIC = $(BIN_DIR)/child_dynamic

all: static shared dynamic

static: $(TARGET_STATIC) $(CHILD_STATIC)

shared: $(TARGET_SHARED) $(CHILD_SHARED)

dynamic: $(TARGET_DYNAMIC) $(CHILD_DYNAMIC) $(LIB_SHARED)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# --- Library generation ---

# Static library objects
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Static library archive
$(LIB_STATIC): $(OBJS_LIB)
	ar rcs $@ $^

# Shared library objects
$(BIN_DIR)/%_shared.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Shared library
$(LIB_SHARED): $(OBJS_LIB_SHARED)
	$(CC) -shared -o $@ $^

# --- Main variants ---

$(TARGET_STATIC): $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DCHILD_PREF=\"./$(CHILD_STATIC)\" -o $@ $<

$(TARGET_SHARED): $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DCHILD_PREF=\"./$(CHILD_SHARED)\" -o $@ $<

$(TARGET_DYNAMIC): $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DCHILD_PREF=\"./$(CHILD_DYNAMIC)\" -o $@ $<

# --- Child variants ---

$(CHILD_STATIC): $(SRC_DIR)/child.c $(LIB_STATIC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< -L$(BIN_DIR) -lhandlers

$(CHILD_SHARED): $(SRC_DIR)/child.c $(LIB_SHARED) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< -L$(BIN_DIR) -lhandlers -Wl,-rpath=$(CURDIR)/$(BIN_DIR)

$(CHILD_DYNAMIC): $(SRC_DIR)/child.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DDYNAMIC -o $@ $< -ldl

clean:
	rm -rf $(BIN_DIR)
