# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN := main

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default rule
all: $(BIN)

# Link the final binary
$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if needed
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
