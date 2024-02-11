`default_nettype none

module processor #(parameter WIDTH = 32) (
    input logic clock,
    input logic reset,
    output logic [WIDTH-1: 0] result        //Not required for functioning, for the sake of an output
);
    logic isBranchR, isBranchA;                  //Branch Result of is it a branching instruction and should branch
    logic isSrc2, isRegWriteE;
    logic [5-1: 0] rs1, rs2, rd, opcode;
    logic [3-1: 0] funct3;
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
    // verilator lint_off UNUSED
    logic [WIDTH-1: 0] instr;   //Used to suppress warning of not using instr[1:0]
    //  verilator lint_on UNUSED

    //PC
    always_ff @(posedge clock) begin : PC
        pc <= (reset) ? 32'd0 : pcNext;
    end
    assign pcPlus4 = pc + 32'd4;  
    //Handling branching and jump
    assign pcNext = (isBranchR | isJAL | isJALR) ? ( (isBranchR)? branchTarget : jumpTarget) : pcPlus4;

    //Instruction memory
    initial begin
        $readmemh("data/ins.dat", memIns);
    end
    assign instr = {memIns[pc+3], memIns[pc+2], memIns[pc+1], memIns[pc]};

    assign funct3 = instr[14:12];
    assign funct7 = instr[31:25];
    assign rs1 = instr[19:15];
    assign rs2 = instr[24:20];
    assign rd  = instr[11:7];
    assign opcode = instr [6:2];

    //Instruction decoder
    assign isArithmetic = (opcode == 5'b01100) & (funct7 == 7'b00000_00);
    assign isImmediate  = (opcode == 5'b00100);
    assign isLoadW      = (opcode == 5'b00000);
    assign isLoadUI     = (opcode == 5'b01101);
    assign isStoreW     = (opcode == 5'b01000);
    assign isBranch     = (opcode == 5'b11000);
    assign isJAL        = (opcode == 5'b11011);
    assign isJALR       = (opcode == 5'b11001);
    assign isMUL        = (opcode == 5'b01100) & (funct7 == 7'b00000_01);     //For MUL and DIV

    //Immediate generation
    assign iImm    = {{21{instr[31]}}, instr[30:20]};                                 
    assign sImm    = {{21{instr[31]}},instr[30:25],instr[11:7]};                      
    assign sbImm   = {{20{instr[31]}},instr[7],instr[30:25],instr[11:8],1'b0};       
    assign uImm    = {instr[31:12],12'b0};                                            
    assign jImm    = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
    always_comb begin : ImmediateGeneration
        case ({isArithmetic , isImmediate , isLoadW , isLoadUI , isStoreW , isBranch , isJAL , isJALR})
            8'b1000_0000: imm = 32'd0;
            8'b0100_0000: imm = iImm;
            8'b0010_0000: imm = iImm;
            8'b0001_0000: imm = uImm;
            8'b0000_1000: imm = sImm;
            8'b0000_0100: imm = sbImm;
            8'b0000_0010: imm = jImm;
            8'b0000_0001: imm = iImm;
            default: imm = 32'd0;
        endcase
    end

    assign branchTarget = pc + {imm[30:0],1'b0};
    assign jumpTarget = pc + imm;

    //Registry
    initial begin
        $readmemh("data/registry.dat", registryFile);
    end
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
        if (isMUL) begin
            case (funct3)
                3'b000: aluOut = src1 * src2;
                3'b100: aluOut = src1 / src2;
                default: aluOut = 32'd0;
            endcase
        end else if (isArithmetic | isImmediate) begin
            case (funct3)
                3'b000: aluOut = (funct7 == 7'b0100000) ? (src1 - src2) : (src1 + src2);
                3'b001: aluOut = src1 << src2[4:0];
                3'b010: aluOut = ($signed(src1) < $signed(src2) ? 32'd1 : 32'd0); 
                3'b011: aluOut = (src1 < src2) ? 32'd1 : 32'd0; 
                3'b100: aluOut = src1 ^ src2;
                3'b101: aluOut = (funct7[6:2] == 5'b01000) ? src1 >>> src2[4:0] : src1 >> src2;
                3'b110: aluOut = src1 | src2;
                3'b111: aluOut = src1 & src2;
                default: aluOut = 32'd0;
            endcase
        end else if (isLoadW | isStoreW) begin
            aluOut = src1 + src2; 
        end else if (isBranch) begin
            case (funct3)
                3'b000: aluOut = (src1 == src2) ? 32'd1 : 32'd0;                    //BEQ
                3'b001: aluOut = src1 - src2;                                       //BNE
                3'b100: aluOut = ($signed(src1) < $signed(src2) ? 32'd1 : 32'd0);   //BLT
                3'b101: aluOut = ($signed(src1) < $signed(src2) ? 32'd0 : 32'd1);   //BGE
                3'b110: aluOut = (src1 < src2) ? 32'd1 : 32'd0;
                3'b111: aluOut =  (src1 < src2) ? 32'd0 : 32'd1;
                default: aluOut = 32'h0;
            endcase
        end else begin
            aluOut = 32'd0;
        end
    end

    //Branch decision
    assign isBranchR = isBranchA & isBranch;    //isBranch is from decoding of instruction
    assign isBranchA = |aluOut;

    //Data memory
    initial begin
        $readmemh("data/data.dat",memData);
    end
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