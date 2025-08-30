#!/usr/bin/env python3
"""
RISC-V Binary to Human-Readable DAT Converter

- Replaces all instructions up to byte 395 with NOPs (0x00000013).
- For each output byte, emits a two-digit hex string on one line.
- Processes all `.bin` files in a folder, writes `.dat` plaintext output with the same basename.

Inputs:
    - `.bin` files in specified folder.
Outputs:
    - `.dat` files with one two-digit hex byte per line.
"""

import struct
from pathlib import Path
import unittest

NOP_INSTR = 0x00000013  # RISC-V "NOP"
INSTR_SIZE = 4
OFFSET = 396  # Start from this byte, keep original. Before, replace with NOP.

def process_binary_file_textual(input_path: Path, output_path: Path):
    """
    Reads a binary file, replaces instructions before OFFSET with NOPs,
    and writes a text file showing the hex value of every byte, one per line.
    """
    with input_path.open("rb") as infile:
        data = infile.read()

    if len(data) % INSTR_SIZE != 0:
        raise ValueError(f"File {input_path} length is not a multiple of 4 bytes.")

    out_bytes = bytearray()
    for i in range(0, len(data), INSTR_SIZE):
        if i < OFFSET:
            out_bytes.extend(struct.pack("<I", NOP_INSTR))
        else:
            out_bytes.extend(data[i:i + INSTR_SIZE])

    with output_path.open("w") as outfile:
        for b in out_bytes:
            outfile.write(f"{b:02x}\n")

def process_folder(folder: Path):
    for bin_file in folder.glob("*.bin"):
        dat_file = bin_file.with_suffix(".dat")
        process_binary_file_textual(bin_file, dat_file)

# ========== UNIT TESTS ==========

class TestDatExport(unittest.TestCase):
    def test_textual_export(self):
        # 100 instructions = 400 bytes; will create various test cases
        input_data = bytearray()
        for i in range(100):
            input_data.extend(struct.pack("<I", i | 0x01000000))  # some nonzero MSBs

        temp_input = Path("test_input.bin")
        temp_output = Path("test_output.dat")

        try:
            with temp_input.open("wb") as f:
                f.write(input_data)

            process_binary_file_textual(temp_input, temp_output)

            # Read the .dat file and check content
            with temp_output.open("r") as f:
                lines = [line.strip() for line in f.readlines()]

            assert len(lines) == len(input_data)
            for b_index in range(len(input_data)):
                # Before OFFSET: should be NOP bytes
                if b_index < OFFSET:
                    # NOP is 0x13 0x00 0x00 0x00 (little-endian order)
                    nop_seq = ['13', '00', '00', '00']
                    self.assertEqual(lines[b_index], nop_seq[b_index % 4])
                else:
                    # After OFFSET: should match bin file
                    expected = f"{input_data[b_index]:02x}"
                    self.assertEqual(lines[b_index], expected)
        finally:
            if temp_input.exists():
                temp_input.unlink()
            if temp_output.exists():
                temp_output.unlink()

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 2:
        folder = Path(sys.argv[1])
        process_folder(folder)
    else:
        unittest.main(argv=[sys.argv[0]])
