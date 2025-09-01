# Set RISCV toolchain path
export PATH := /home/anuki/comparch/riscv/bin:$(PATH)

# Sources
C_SRC = program.c
ASM_SRC = start.S
ALL_SRC = $(C_SRC) $(ASM_SRC)

# Output names
BASE = program
OBJ = $(BASE).o start.o program.o
ELF = $(BASE).elf
BIN = $(BASE).bin
MEM = $(BASE).txt

# Linker script
LDS = sections.lds

# Default target: build everything
all: $(MEM)

# Step 1: generate assembly from C (optional)
program.S: program.c
	riscv32-unknown-elf-gcc -S -march=rv32i -mabi=ilp32 -I../user_code -o program.S program.c

# Step 2: assemble all .S files
start.o: start.S
	riscv32-unknown-elf-as -march=rv32i -o start.o start.S

program.o: program.S
	riscv32-unknown-elf-as -march=rv32i -o program.o program.S

# Step 3: link using custom linker script
$(ELF): start.o program.o $(LDS)
	riscv32-unknown-elf-ld -T $(LDS) -o $(ELF) start.o program.o

# Step 4: generate binary
$(BIN): $(ELF)
	riscv32-unknown-elf-objcopy -O binary $(ELF) $(BIN)

# Step 5: generate memory file for simulation
$(MEM): $(BIN)
	hexdump -v -e '1/1 "%02x\n"' $(BIN) > $(MEM)

# Dump assembly instructions (new target)
dump: $(ELF)
	riscv32-unknown-elf-objdump -d $(ELF) > program.dump

# Clean
clean:
	rm -f $(OBJ) $(ELF) $(BIN) $(MEM) program.S program.dump

