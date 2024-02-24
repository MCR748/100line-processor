`default_nettype none

module processor #(parameter WIDTH = 32) (
    input logic clock,
    input logic reset,
    output logic [WIDTH-1: 0] result        //Not required for functioning, for the sake of an output
);
    logic isBranchC, isBranchR;                  //Branch Result of is it a branching instruction and should branch
    logic isSrc2, regWrite;
    logic [5-1: 0] rs1, rs2, rd, opcode;
    logic [3-1: 0] funct3;
    /* verilator lint_off UNUSED */
    logic [WIDTH-1: 0] instruction;
    logic [7-1: 0] funct7;
    /* verilator lint_on UNUSED */
    logic [WIDTH-1: 0] instructionMemory [0 :512-1];
    logic [8-1: 0] dataMemory [0 :256-1];

    logic [WIDTH-1:0] pc, pcPlus4, pcNext;
    logic [WIDTH-1: 0] data_1, data_2, regData;
    logic [WIDTH - 1: 0] registers [0 :32-1];
    logic [WIDTH-1: 0] src1, src2, aluOut = 32'd0;
    logic [WIDTH-1: 0] dataMemOut;
    logic isArithmetic, isImmediate, isLoadW, isLoadUI, isStoreW, isBranch, isJAL, isJALR, isMUL, isAUIPC;
    logic [WIDTH-1: 0] imm, iImm, sImm, sbImm, uImm, jImm;
    logic [4-1: 0] aluOp = 4'b0;
    
    always_ff @(posedge clock) begin : PC
        pc <= (reset) ? 32'd0 : pcNext;
    end
    assign pcPlus4 = {pc[31:2], 2'b00} + 32'd4;  
    //Handling branching and jump
    assign pcNext = (isJAL | isJALR  | isBranchR) ? aluOut : pcPlus4;

    //Instruction memory
    initial $readmemh("data/ins.dat", instructionMemory);
    assign instruction = instructionMemory[pc[10:2]];

    assign funct3 = instruction[14:12];
    assign funct7 = instruction[31:25];
    assign rs1 = instruction[19:15];
    assign rs2 = instruction[24:20];
    assign rd  = instruction[11:7];
    assign opcode = instruction [6:2];

    //Instruction decoder
    assign isArithmetic = (opcode == 5'b01100) & (funct7[0] == 1'b0);
    assign isImmediate  = (opcode == 5'b00100);
    assign isLoadW      = (opcode == 5'b00000);
    assign isLoadUI     = (opcode == 5'b01101);
    assign isStoreW     = (opcode == 5'b01000);
    assign isBranch     = (opcode == 5'b11000);
    assign isJAL        = (opcode == 5'b11011);
    assign isJALR       = (opcode == 5'b11001);
    assign isMUL        = (opcode == 5'b01100) & (funct7[0] == 1'b1);     //For MUL and DIV
    assign isAUIPC      = (opcode == 5'b00101);

    //Immediate generation
    assign iImm    = {{21{instruction[31]}}, instruction[30:20]};                                 
    assign sImm    = {{21{instruction[31]}},instruction[30:25],instruction[11:7]};                      
    assign sbImm   = {{20{instruction[31]}},instruction[7],instruction[30:25],instruction[11:8],1'b0};       
    assign uImm    = {instruction[31:12],12'b0};                                            
    assign jImm    = {{12{instruction[31]}}, instruction[19:12], instruction[20], instruction[30:21], 1'b0};
    always_comb begin : ImmediateGeneration
        case ({isImmediate , isLoadW , isLoadUI , isStoreW , isBranch , isJAL , isJALR, isAUIPC})
            8'b1000_0000: imm = iImm;
            8'b0100_0000: imm = iImm;
            8'b0010_0000: imm = uImm;
            8'b0001_0000: imm = sImm;
            8'b0000_1000: imm = sbImm;
            8'b0000_0100: imm = jImm;
            8'b0000_0010: imm = iImm;
            8'b0000_0001: imm = uImm;
            default: imm = 32'd0;
        endcase
    end

    //Registry
    initial $readmemh("data/registry.dat", registers);
    assign data_1 = (rs1 == 0) ? 0 : registers[rs1];
    assign data_2 = (rs2 == 0) ? 0 : registers[rs2];
    assign regWrite = isArithmetic | isImmediate | isLoadW | isLoadUI | isJAL | isJALR | isAUIPC;
    always_ff @(posedge clock) begin : RegistryFile
         if (regWrite) registers[rd] <= regData;
    end

    //ALU
    assign isSrc2 = isImmediate | isLoadW | isLoadUI | isJAL | isJALR | isStoreW | isBranch | isAUIPC; 
    assign src1 = (isJAL | isBranch | isAUIPC) ? pc : data_1;
    assign src2 = (isSrc2)  ? imm : data_2;
    always_comb begin : ALU
        case (aluOp)
            4'b0000 : aluOut = src1 + src2;  //Add
            4'b1000 : aluOut = src1 - src2;  //Sub
            4'b0001 : aluOut = src1 << src2[4:0];  //Sll
            4'b0010 : aluOut = ($signed(src1) < $signed(src2) ? 32'd1 : 32'd0);  //Slt
            4'b0011 : aluOut = (src1 < src2) ? 32'd1 : 32'd0;  //Sltu
            4'b0100 : aluOut = src1 ^ src2;  //Xor
            4'b0101 : aluOut = src1 >> src2[4:0];  //Srl
            /* verilator lint_off WIDTH */
            4'b1101 : aluOut =  {{31{src1[31]}}, src1} >> src2[4:0];  //Sra, normal operation >>> didn't work
            /* verilator lint_on UNUSED */
            4'b0110 : aluOut = src1 | src2;  //Or
            4'b0111 : aluOut = src1 & src2;  //And
            4'b1001 : aluOut = src1 * src2;  //Mul
            4'b1010 : aluOut = src1 / src2;  //Div
            4'b1111 : aluOut = src2;        //Passthrough
            default:  aluOut = 32'd0;
        endcase
    end  
    always_comb begin : ALUControl
        if (isMUL) begin
            aluOp = (funct3[2] ? 4'b1010 : 4'b1001);
        end else if (isArithmetic) begin
            aluOp = {funct7[5], funct3};
        end else if (isImmediate) begin
            aluOp = ({funct3 == 3'b101}) ? {funct7[5], funct3} : {1'b0, funct3}; //Fro SRAI and SRLI
        end else if (isAUIPC | isJAL | isJALR | isBranch | isLoadW | isStoreW) begin
            aluOp = 4'b0000;   //Can put with load and store            
        end else begin
            aluOp =  4'b1111;
        end
    end

    //Branch decision
    always_comb begin : BranchComparator
        case (funct3[2:1])
            2'b00: isBranchC = (funct3[0]) ^ (data_1 == data_2);                    //BNE, BEQ 
            2'b10: isBranchC = (funct3[0]) ^ ($signed(data_1) < $signed(data_2));   //BLT, BGE
            2'b11: isBranchC = (funct3[0]) ^ (data_1 < data_2);                     //BLTU, BGEU
            default: isBranchC = 1'b0; 
        endcase
    end
    assign isBranchR = isBranchC & isBranch;    //isBranch is from decoding of instruction

    //Data memory
    initial $readmemh("data/data.dat",dataMemory);
    always_ff @(posedge clock) begin : DataMemory
        if (isStoreW) begin
            dataMemory[aluOut]   <= data_2[7:0];
            dataMemory[aluOut+1] <= data_2[15:8];
            dataMemory[aluOut+2] <= data_2[23:16];
            dataMemory[aluOut+3] <= data_2[31:24];
        end
    end
    assign dataMemOut =  {dataMemory[aluOut+3], dataMemory[aluOut+2], dataMemory[aluOut+1], dataMemory[aluOut]};
    assign result = dataMemOut;     //Just to get an output

    //Writeback Mux
    assign regData = (isJALR | isJAL) ? pc + 4 : ((isLoadW) ? dataMemOut : aluOut);  
endmodule