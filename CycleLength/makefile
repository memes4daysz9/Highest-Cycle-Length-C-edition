# Compiler and flags
CXX := g++
CXXFLAGS := -Ofast -flto -fomit-frame-pointer -march=native -funroll-loops -ffast-math -fno-math-errno -fno-trapping-math -fno-signaling-nans -falign-functions=64 -DNDEBUG -fprefetch-loop-arrays -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -free -fipa-icf -floop-parallelize-all -ftree-loop-distribution -floop-interchange -fno-signed-zeros -fno-trapping-math -frounding-math -fsplit-loops -fversion-loops-for-strides -fstdarg-opt 
LDFLAGS = -pg

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