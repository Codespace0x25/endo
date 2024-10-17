# Makefile for Bare-Bones OS

# Compiler and tools
CC = gcc
ASM = nasm
LD = ld

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include

# Source files
ASM_SRC = $(SRC_DIR)/boot.asm $(SRC_DIR)/KeybardS.asm $(SRC_DIR)/IDTS.asm
C_SRC = $(SRC_DIR)/Display.c $(SRC_DIR)/kernel.c $(SRC_DIR)/System.c

# Object files
ASM_OBJ = $(BUILD_DIR)/boot.o $(BUILD_DIR)/KeybardS.o $(BUILD_DIR)/IDTS.o
C_OBJ = $(BUILD_DIR)/Display.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/System.o 

# Final binary
OUTPUT = os_image.bin

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR) -ffreestanding -m32
LDFLAGS = -T linker.ld -m elf_i386

# Build rules
all: $(OUTPUT)

$(OUTPUT): $(C_OBJ) $(ASM_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) -f elf $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(OUTPUT)

run:$(OUTPUT)
	qemu-system-i386 -kernel $(OUTPUT)
.PHONY: all clean

