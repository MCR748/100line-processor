# Tool configuration
OBJCOPY = /mnt/Education/RISC-V_toolchain/riscv/bin/riscv32-unknown-elf-objcopy
OBJCOPYFLAGS = -O binary
HEXDUMP = hexdump -v -e '1/4 "%032b\\n"'

# List all files with no extension (excluding .bin, .mem, and Makefile)
ELF_FILES := $(basename $(filter-out %.bin,$(filter-out %.mem,$(filter-out Makefile,$(shell find . -maxdepth 1 -type f ! -name "*.*" -exec basename {} \;)))))
# Generate .bin and .mem file names from base names
BIN_FILES := $(addsuffix .bin,$(ELF_FILES))
MEM_FILES := $(addsuffix .mem,$(ELF_FILES))

# Add: Find all .bin files for dat target
BIN_TARGETS := $(basename $(filter %.bin,$(shell find . -maxdepth 1 -type f -name "*.bin")))
DAT_FILES := $(addsuffix .dat,$(BIN_TARGETS))

.PHONY: all clean dat bin

# Main target: generate both bin and mem files
all: $(MEM_FILES)

# Convert file with no extension to .bin
%.bin: %
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

# Convert .bin to .mem using hexdump
%.mem: %.bin
	$(HEXDUMP) $< > $@

# Convert all .bin files to .dat files
dat: $(DAT_FILES)

# Actual rule: .dat from .bin for dat target
%.dat: %.bin
	$(HEXDUMP) $< > $@

# Create only bin files from extension-less ELF files
bin: $(BIN_FILES)

clean:
	rm -f $(BIN_FILES) $(MEM_FILES) $(DAT_FILES)
