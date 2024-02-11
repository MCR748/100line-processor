#To convert a set of instructions in the a text file to the required order.

# Open the input file for reading and the output file for writing
with open('/media/Education/Project-self/Comp-arch-course/100linev2/data/input.dat', 'r') as infile, open('/media/Education/Project-self/Comp-arch-course/100linev2/data/ins.dat', 'w') as outfile:
    # Iterate over each line in the input file
    for line in infile:
        # Strip leading/trailing whitespace and remove the '0x' prefix if present
        stripped_line = line.strip().lstrip('0x')
        
        # Convert the hexadecimal string to an integer
        instruction_int = int(stripped_line,  16)
        
        # Convert the integer to bytes (4 bytes for a  32-bit RISC-V instruction)
        instruction_bytes = instruction_int.to_bytes(4, 'big')

        # Reverse the byte order
        reversed_bytes = instruction_bytes[::-1]
        
        # Write each byte to the output file as a separate line, padded with zeros
        for byte in reversed_bytes:
            outfile.write(f"{byte:02x}\n")
