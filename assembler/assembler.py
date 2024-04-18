import sys

NUM_GPR = 32
opcodes = {
    'ADD'  : ['0110011','000', '0000000'],
    'SUB'  : ['0110011','000', '0100000'],
    'AND'  : ['0110011','111', '0000000'],
    'OR'   : ['0110011','110', '0000000'],
    'XOR'  : ['0110011','100', '0000000'],
    'SLL'  : ['0110011','001', '0000000'],
    'SRL'  : ['0110011','101', '0000000'],
    'SRA'  : ['0110011','101', '0100000'],
    'SLT'  : ['0110011','010', '0000000'],
    'SLTU' : ['0110011','011', '0000000'],
    'MUL'  : ['0110011','000', '0000001'],
    'DIV'  : ['0110011','100', '0000001'],
    'ADDI' : ['0010011','000'],
    'ANDI' : ['0010011','000'],
    'ORI'  : ['0010011','111'],
    'XORI' : ['0010011','110'],
    'SLLI' : ['0010011','100', '0000000'],
    'SRLI' : ['0010011','001', '0000000'],
    'SRAI' : ['0010011','101', '0100000'],
    'SLTI' : ['0010011','101'],
    'SLTIU': ['0010011','010'],
    'LW'   : ['0000011','010'],
    'SW'   : ['0100011','010'],
    'BEQ'  : ['1100011','000'],
    'BNE'  : ['1100011','001'],
    'BLT'  : ['1100011','100'],
    'BGE'  : ['1100011','101'],
    'BLTU' : ['1100011','110'],
    'BGEU' : ['1100011','111'],
    'LUI'  : ['0110111'],
    'AUIPC': ['0010111'],
    'JAL'  : ['1101111'],
    'JALR' : ['1100111','000']
}
registers = ['ZERO','RA','SP','GP','TP','T0','T1','T2','S0','S1','A0','A1','A2','A3','A4','A5','A6','A7','S2','S3','S4','S5','S6','S7','S8','S9','S10','S11','T3','T4','T5','T6']

'''
Setup
'''
in_filename = sys.argv[1]
#out_filename = in_filename.replace('assembly', 'mcode')
out_filename = 'input.dat'

input_lines = []
with open(in_filename, "r") as f:
    input_lines = f.readlines()

program = []
jump_labels = {}
variables = {}
iram_addr = 0

'''
Read & parse assembly, identify jump labels
'''
for i, line in enumerate(input_lines):
    print("Start of new instruction")
    print(i)      #debug
    print(line)   #debug
    ins = line.split("#")[0] # discard comments
    print(ins)   #debug

    if ins.isspace() or ins == "":
        continue # skip blank input_lines
    
    '''
    Remove tabs, commas, newline, extra spaces
    '''
    ins = ins.strip().replace("\t", "").replace(",", "")
    print(ins)   #debug
    ins = ' '.join(ins.split())
    print(ins)   #debug
    ins = ins.split()
    print(ins)   #debug
    
    '''
    Remove & store jump labels
    '''
    if ins[0][0] == "$":
        print("Label Detected")  #debug
        label, *ins = ins
        jump_labels[label.upper()] = iram_addr

    if ins[0][0] == "`":
        print("Variable Detected") #debug
        reg, var = ins
        variables[var] = reg[1:]
        continue
    else:
        for j, word in enumerate(ins):
            if word in variables.keys():
                ins[j] = variables[word]

    '''
    Add line number (for debugging), and convert all to uppercase
    '''
    print(i)     #debug
    ins = [iram_addr] + [word.upper() for word in ins]
    print(ins)   #debug

    program += [ins]
    iram_addr += 1

    #print(ins)
    print("End of a Instruction")



print('\n')                                     #debug
print('start of conversion begins')             #debug
print('\n')                                     #debug
'''
Check Syntax Errors & Translate to Machine code
'''
with open(out_filename, "w") as f:
    for ins in program:
        binary = ""
        iram_addr, opcode, *operands = ins 
        header = f"Syntax error, line:{iram_addr} ->"
        assert opcode in opcodes, f"{header} Invalid opcode '{opcode}'. Valid:{opcodes}"


        if opcode in ['ADD', 'SUB', 'AND', 'OR', 'XOR', 'SLL', 'SRL', 'SRA', 'SLT', 'SLTU', 'MUL', 'DIV']:
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            funct7 = opcodes[opcode][2]
            assert len(operands) == 3, f"{header} Need 3 operands for {opcode}"
            binary += f"{funct7}"
            binary += f"{registers.index(operands[2]):05b}"
            binary += f"{registers.index(operands[1]):05b}"
            binary += f"{funct3}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            
        
        if opcode in ['ADDI','ANDI','ORI','XORI','SLTI','SLTIU']:
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            assert len(operands) == 3, f"{header} Need 3 operands for {opcode}"
            imm = int(operands[2])
            imm = imm if imm >=0 else imm + 4096 # 2's complement
            binary += f"{imm:012b}"
            binary += f"{registers.index(operands[1]):05b}"
            binary += f"{funct3}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
           
            
        if opcode in ['SLLI', 'SRLI', 'SRAI']:
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            funct7 = opcodes[opcode][2]
            assert len(operands) == 3, f"{header} Need 3 operands for {opcode}"
            imm = int(operands[2])
            assert imm < 32, f"shift amount should be less than 32"
            binary += f"{funct7}"
            binary += f"{imm:05b}"
            binary += f"{registers.index(operands[1]):05b}"
            binary += f"{funct3}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            

        #from here onwards changes need to be done
        if opcode in ['BEQ', 'BNE','BLT','BGE','BLTU','BGEU']:
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            assert len(operands) == 3, f"{header} Need 3 operands for {opcode}"
            imm = operands[2]
            if not imm.isnumeric():
                assert imm in jump_labels.keys(), f"{header} Invalid jump label '{imm}'. Available:{jump_labels}"  
                imm = (int(jump_labels[imm]) - iram_addr)*4 #PC relative addressing
                imm = imm if imm >=0 else imm + 8196    # 2's complement
            else:
                imm = (int(imm) - iram_addr)*4              #PC relative addressing
                imm = imm if imm >=0 else imm + 8196    # 2's complement
            imm = bin(imm)
            imm = imm[2:].zfill(13)
            binary += f"{imm[0]+imm[2:8]}"
            binary += f"{registers.index(operands[1]):05b}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += f"{funct3}"
            binary += f"{imm[8:12]+imm[1]}"
            binary += opcode_bits
            


        if opcode =='SW':
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            assert len(operands) == 2, f"{header} Need 2 operands for {opcode}"
            imm = int(operands[1].split('(')[0])
            imm = bin(imm)[2:].zfill(12) if imm >=0 else bin(imm + 4096)[2:].zfill(12) # 2's complement
            binary += f"{imm[0:7]}"
            binary += f"{registers.index(operands[1].split('(')[1].rstrip(')')):05b}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += f"{funct3}"
            binary += f"{imm[7:]}"
            binary += opcode_bits
            

        if opcode == 'LW':
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            imm = int(operands[1].split('(')[0])
            imm = imm if imm >=0 else imm + 4096 # 2's complement
            binary += f"{imm:012b}"
            binary += f"{registers.index(operands[1].split('(')[1].rstrip(')')):05b}"
            binary += f"{funct3}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            

        if opcode == 'LUI':
            opcode_bits = opcodes[opcode][0]
            assert len(operands) == 2, f"{header} Need 2 operand for {opcode}"
            imm = int(operands[1])
            imm = bin(imm)[2:].zfill(20) if imm >=0 else bin(imm + 1048576)[2:].zfill(20) # 2's complement
            binary += f"{imm}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            
            
        if opcode == 'AUIPC':
            opcode_bits = opcodes[opcode][0]
            assert len(operands) == 2, f"{header} Need 2 operand for {opcode}"
            imm = int(operands[1])
            imm = bin(imm)[2:].zfill(20) if imm >=0 else bin(imm + 1048576)[2:].zfill(20) # 2's complement
            binary += f"{imm}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            
        
        if opcode == 'JAL':
            opcode_bits = opcodes[opcode][0]
            assert len(operands) == 2, f"{header} Need 2 operand for {opcode}"
            imm = int(operands[1])
            imm = bin(imm)[2:].zfill(21) if imm >=0 else bin(imm + 2097152)[2:].zfill(21) # 2's complement
            binary += f"{imm[0]+imm[10:20]+imm[9]+imm[1:9]}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            
            
        if opcode in ['JALR']:
            opcode_bits = opcodes[opcode][0]
            funct3 = opcodes[opcode][1]
            assert len(operands) == 2, f"{header} Need 2 operands for {opcode}"
            imm = int(operands[1].split('(')[0])
            imm = imm if imm >=0 else imm + 4096 # 2's complement
            binary += f"{imm:012b}"
            binary += f"{registers.index(operands[1].split('(')[1].rstrip(')')):05b}"
            binary += f"{funct3}"
            binary += f"{registers.index(operands[0]):05b}"
            binary += opcode_bits
            
        
        decimal_int = int(binary, 2)
        hex_str = '0x{:08x}'.format(decimal_int)
        print(hex_str)
        f.write(hex_str[2:] + '\n')
        