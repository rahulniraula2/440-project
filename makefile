# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
ASMFLAGS = -S

# Optimization flag default
OPTIMIZATION = -O2

# Source and target directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
ASM_DIR = $(BUILD_DIR)/asm

# Target executable name
TARGET = $(BIN_DIR)/main_program

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

@echo "INFO: Using -O$(OPTIMIZATION) optimization level"

# Default target
all: clean build assembly run

# Build target
build: $(TARGET)

# Link target
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "/--------------------------------\\"
	@echo "INFO: Linking object files to create the executable"
	$(CC) $(CFLAGS) $(OPTIMIZATION) -o $@ $^
	@echo "\\--------------------------------/\n"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "/--------------------------------\\"
	@echo "INFO: Compiling $< to object file"
	$(CC) $(CFLAGS) $(OPTIMIZATION) -c $< -o $@
	@echo "\\--------------------------------/\n"

# Create necessary directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) 

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(ASM_DIR):
	@mkdir -p $(ASM_DIR) 

# Clean up build files
clean:
	@echo "/--------------------------------\\"
	@echo "INFO: Cleaning up build files"
	rm -rf $(BUILD_DIR)
	@echo "\\--------------------------------/\n"

# Run the program
run: $(TARGET)
	@echo "/--------------------------------\\"
	@echo "INFO: Running the program"
	@echo "\\--------------------------------/\n"
	@./$(TARGET)

# Build assembly
assembly: $(SRCS) | $(ASM_DIR)
	@echo "/--------------------------------\\"
	@echo "INFO: Generating assembly files"
	@for src in $^; do \
		echo "INFO: Generating assembly for $$src"; \
		$(CC) $(ASMFLAGS) $(OPTIMIZATION) $$src -o $(ASM_DIR)/$$(basename $$src .c).s; \
	done
	@echo "\\--------------------------------/\n"

.PHONY: all build clean run assembly

# Parse the -o flag for optimization level
ifneq ($(filter -o%, $(MAKEFLAGS)),)
  OPT_LEVEL := $(patsubst -o%,%,$(filter -o%, $(MAKEFLAGS)))
  OPTIMIZATION := -O$(OPT_LEVEL)
  $(info INFO: Setting optimization level to $(OPTIMIZATION))
endif
