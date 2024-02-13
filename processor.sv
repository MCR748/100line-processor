`default_nettype none

module processor #(parameter WIDTH = 32) (
    input logic clock,
    input logic reset,
    output logic [WIDTH-1: 0] result        //Not required for functioning, for the sake of an output
);
    logic isBranchA, isBranchR;                  //Branch Result of is it a branching instruction and should branch
    logic isSrc2, isRegWriteE;
    logic [5-1: 0] rs1, rs2, rd, opcode;
    logic [3-1: 0] funct3;
    logic [WIDTH-1: 0] instr;
    logic [7-1: 0] funct7;
    logic [8-1: 0] memIns [0 :256-1];
    logic [8-1: 0] memData [0 :256-1];

    logic [WIDTH-1:0] pc, pcPlus4, pcNext;
    logic [WIDTH-1:0] branchTarget, jumpTarget;
    logic [WIDTH-1: 0] data_1, data_2, regData;
    logic [WIDTH - 1: 0] registryFile [0 :32-1];
    logic [WIDTH-1: 0] src1, src2, aluOut = 32'd0;
    logic [WIDTH-1: 0] dataMemOut;
    logic isArithmetic, isImmediate, isLoadW, isLoadUI, isStoreW, isBranch, isJAL, isJALR, isMUL;
    logic [WIDTH-1: 0] imm, iImm, sImm, sbImm, uImm, jImm;
    logic [4-1: 0] aluControl = 4'b0;
    
    //PC
    always_ff @(posedge clock) begin : PC
        pc <= (reset) ? 32'd0 : pcNext;
    end
    assign pcPlus4 = pc + 32'd4;  
    //Handling branching and jump
    assign pcNext = (isBranchR | isJAL | isJALR) ? ( (isBranchR)? branchTarget : jumpTarget) : pcPlus4;

    //Instruction memory
    initial $readmemh("data/ins.dat", memIns);
    assign instr = {memIns[pc+3], memIns[pc+2], memIns[pc+1], memIns[pc]};

    assign funct3 = instr[14:12];
    assign funct7 = instr[31:25];
    assign rs1 = instr[19:15];
    assign rs2 = instr[24:20];
    assign rd  = instr[11:7];
    assign opcode = instr [6:2];

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

    //Immediate generation
    assign iImm    = {{21{instr[31]}}, instr[30:20]};                                 
    assign sImm    = {{21{instr[31]}},instr[30:25],instr[11:7]};                      
    assign sbImm   = {{20{instr[31]}},instr[7],instr[30:25],instr[11:8],1'b0};       
    assign uImm    = {instr[31:12],12'b0};                                            
    assign jImm    = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
    always_comb begin : ImmediateGeneration
        case ({isImmediate , isLoadW , isLoadUI , isStoreW , isBranch , isJAL , isJALR})
            7'b100_0000: imm = iImm;
            7'b010_0000: imm = iImm;
            7'b001_0000: imm = uImm;
            7'b000_1000: imm = sImm;
            7'b000_0100: imm = sbImm;
            7'b000_0010: imm = jImm;
            7'b000_0001: imm = iImm;
            default: imm = 32'd0;
        endcase
    end

    assign branchTarget = pc + {imm[31:1],1'b0};
    assign jumpTarget = pc + imm;

    //Registry
    initial $readmemh("data/registry.dat", registryFile);
    assign data_1 = (rs1 == 0) ? 0 : registryFile[rs1];
    assign data_2 = (rs2 == 0) ? 0 : registryFile[rs2];
    assign isRegWriteE = isArithmetic | isImmediate | isLoadW | isLoadUI | isJAL | isJALR;
    always_ff @(posedge clock) begin : RegistryFile
         if (isRegWriteE) registryFile[rd] <= regData;
    end

    //ALU
    assign isSrc2 = isImmediate | isLoadW | isLoadUI | isJAL | isJALR | isStoreW; 
    assign src1 = data_1;
    assign src2 = (isSrc2) ? imm : data_2;
    always_comb begin : ALU
        case (aluControl)
            4'b0000 : aluOut = src1 + src2;  //Add
            4'b1000 : aluOut = src1 - src2;  //Sub
            4'b0001 : aluOut = src1 << src2[4:0];  //Sll
            4'b0010 : aluOut = ($signed(src1) < $signed(src2) ? 32'd1 : 32'd0);  //Slt
            4'b0011 : aluOut = (src1 < src2) ? 32'd1 : 32'd0;  //Sltu
            4'b0100 : aluOut = src1 ^ src2;  //Xor
            4'b0101 : aluOut = src1 >> src2;  //Srl
            4'b1101 : aluOut = src1 >>> src2[4:0];  //Sra
            4'b0110 : aluOut = src1 | src2; //Or
            4'b0111 : aluOut = src1 & src2;  //And
            4'b1001 : aluOut = src1 * src2;  //Mul
            4'b1010 : aluOut = src1 / src2;   //Div
            4'b1011 : aluOut = (src1 == src2) ? 32'd1 : 32'd0; //Equal
            4'b1100 : aluOut = ($signed(src1) < $signed(src2) ? 32'd0 : 32'd1);   //BGE
            4'b1110 : aluOut = (src1 < src2) ? 32'd0 : 32'd1;        //BGEU
            4'b1111 : aluOut = src2;        //Passthrough
            default:  aluOut = 32'd0;
        endcase
    end  
    always_comb begin : ALUDecode
        if (isMUL) begin
            aluControl = (funct3[2] ? 4'b1010 : 4'b1001);
        end else if (isArithmetic) begin
            aluControl = {funct7[5], funct3};
        end else if (isImmediate) begin
            aluControl = {1'b0, funct3};
        end else if (isLoadW | isStoreW) begin
            aluControl = 4'b0000; 
        end else if (isBranch) begin
            case (funct3)
                3'b000:  aluControl = 4'b1011;   //BEQ
                3'b001:  aluControl = 4'b1000;   //BNE
                3'b100:  aluControl = 4'b0010;   //BLT
                3'b101:  aluControl = 4'b1100;   //BGE
                3'b110:  aluControl = 4'b0011;   //BLTU
                3'b111:  aluControl = 4'b1110;   //BGEU
                default: aluControl = 4'b1111;
            endcase
        end else begin
            aluControl =  4'b1111;
        end
    end

    //Branch decision
    assign isBranchR = isBranchA & isBranch;    //isBranch is from decoding of instruction
    assign isBranchA = |aluOut;

    //Data memory
    initial $readmemh("data/data.dat",memData);
    always_ff @(posedge clock) begin : DataMemory
        if (isStoreW) begin
            memData[aluOut]   <= data_2[7:0];
            memData[aluOut+1] <= data_2[15:8];
            memData[aluOut+2] <= data_2[23:16];
            memData[aluOut+3] <= data_2[31:24];
        end
    end
    assign dataMemOut =  {memData[aluOut+3], memData[aluOut+2], memData[aluOut+1], memData[aluOut]};
    assign result = dataMemOut;     //Just to get an output

    //Writeback Mux
    assign regData = (isJALR | isJAL) ? pc + 4 : ((isLoadW | isLoadUI ) ? dataMemOut : aluOut);     
endmodule