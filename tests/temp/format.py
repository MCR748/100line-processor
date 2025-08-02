import os

folder_path = "/mnt/Education/Project-self/Comp-arch-revive/100line-processor/tests"

for file_name in os.listdir(folder_path):
    if file_name.endswith('.dump'):
        full_path = os.path.join(folder_path, file_name)
        out_path = os.path.join(folder_path, file_name + '.dat')
        
        with open(full_path, 'r') as f_in, open(out_path, 'w') as f_out:
            # Write initial NOP (0x00000013) in little-endian
            count = 0
            nop = int("00000013", 16)
            for byte in nop.to_bytes(4, byteorder='little'):
                f_out.write(f"{byte:02x}\n")

            for line in f_in:
                if len(line) > 8 and line[8] == ':':
                    count += 1
                    instr_str = line[10:18]
                    if count > 396/4 :
                        if len(instr_str) == 8 and all(c in '0123456789abcdefABCDEF' for c in instr_str):
                            try:
                                instr = int(instr_str, 16)
                                for byte in instr.to_bytes(4, byteorder='little'):
                                    f_out.write(f"{byte:02x}\n")
                            except ValueError:                            continue  # skip malformed lines
