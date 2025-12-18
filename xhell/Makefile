CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET = xhell

# Default target
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Clean complete"

# Rebuild
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
