//`default_nettype none

module processor #(
    WIDTH = 32, IMEM_DEPTH=512, DMEM_DEPTH=32, NUM_REGS=32
)(
    input  logic clock, reset, insMemEn,
    input  logic [WIDTH-1:0] insMemData, insMemAddr,
    output logic [WIDTH-1:0] gp, a7, dataMemOut //verifying
);
    logic [DMEM_DEPTH-1:0][WIDTH-1:0] dataMemory;
    logic [IMEM_DEPTH-1:0][WIDTH-1:0] insMemory ;
    logic [NUM_REGS  -1:0][WIDTH-1:0] registers ;

    logic [3:0] aluOp;
    logic [4:0] rs1, rs2, rd, opcode;
    logic [2:0] funct3;
    logic [6:0] funct7;
    logic [WIDTH-1:0] ins, imm, iImm, sImm, sbImm, uImm, jImm, pc, pcPlus4, data_1, data_2, regData, src1, src2, aluOut;
    logic isArithmetic, isImmediate, isLoadW, isLoadUI, isStoreW, isBranch, isJAL, isJALR, isMUL, isAUIPC, isBranchC, isBranchR, regWrite;
    
    //PC
    always_ff @(posedge clock)
        if (reset) pc <= 0;
        else       pc <= (isJAL|isJALR|isBranchR) ? aluOut : (pc + 4);

    //Instruction memory
    always_ff @(posedge clock) //initial $readmemh("tests/rv32ui-p-lui.dump.dat", insMemory);
        insMemory[insMemAddr[8:0]] <= (insMemEn) ? insMemData : insMemory[insMemAddr[8:0]];

    assign ins = insMemEn ? 32'h13 : insMemory[pc[10:2]];
    assign {funct7, rs2, rs1, funct3, rd, opcode} = ins[31:2];

    always_comb begin
        //ins decoder
        isArithmetic = (opcode == 5'b01100) & (funct7[0] == 1'b0);
        isMUL        = (opcode == 5'b01100) & (funct7[0] == 1'b1); //For MUL and DIV
        isImmediate  = (opcode == 5'b00100);
        isLoadW      = (opcode == 5'b00000);
        isLoadUI     = (opcode == 5'b01101);
        isStoreW     = (opcode == 5'b01000);
        isBranch     = (opcode == 5'b11000);
        isJAL        = (opcode == 5'b11011);
        isJALR       = (opcode == 5'b11001);
        isAUIPC      = (opcode == 5'b00101);

        //Immediate generation
        uImm  = {ins[31:12], 12'b0};
        iImm  = WIDTH'($signed( ins[31:20]));
        sImm  = WIDTH'($signed({ins[31:25], ins[11:7]}));                      
        sbImm = WIDTH'($signed({ins[31]   , ins[7]    , ins[30:25], ins[11:8] , 1'b0}));       
        jImm  = WIDTH'($signed({ins[31]   , ins[19:12], ins[20]   , ins[30:21], 1'b0}));

        if (isImmediate|isLoadW|isJALR) imm = iImm;
        else if (isLoadUI|isAUIPC)      imm = uImm;
        else if (isStoreW)              imm = sImm;
        else if (isBranch)              imm = sbImm;
        else if (isJAL)                 imm = jImm;
        else                            imm = 0;
    end

    //Registers
    //initial $readmemh("data/registry.dat", registers);
    assign data_1 = (rs1 == 0) ? 0 : registers[rs1];
    assign data_2 = (rs2 == 0) ? 0 : registers[rs2];
    assign regWrite = isArithmetic | isImmediate | isLoadW | isLoadUI | isJAL | isJALR | isAUIPC;

    always_ff @(posedge clock)
        if (regWrite) registers[rd] <= regData;
        

    //ALU
    localparam [3:0] ADD=0, SLL=1, SLT=2, SLTU=3, XOR=4, SRL=5, OR =6, AND=7, SUB=8, MUL=9, DIV=10, SRA=13, PASS=15;

    always_comb begin
        if      (isMUL)                                          aluOp = (funct3[2] ? DIV : MUL);
        else if (isArithmetic)                                   aluOp = {funct7[5], funct3};
        else if (isImmediate)                                    aluOp = (funct3 == 3'b101) ? {funct7[5], funct3} : {1'b0, funct3};
        else if (isAUIPC|isJAL|isJALR|isBranch|isLoadW|isStoreW) aluOp = ADD ;   //Can put with load and store
        else                                                     aluOp = PASS;

        src1 = (isJAL|isBranch|isAUIPC) ? pc : data_1;
        src2 = (isImmediate|isLoadW|isLoadUI|isJAL|isJALR|isStoreW|isBranch|isAUIPC) ? imm : data_2;

        unique case (aluOp)
            ADD    : aluOut = src1 + src2;
            SUB    : aluOut = src1 - src2;                                      
            SLL    : aluOut = src1 << src2[4:0];                                
            SLT    : aluOut = WIDTH'($signed  (src1) < $signed  (src2));
            SLTU   : aluOut = WIDTH'($unsigned(src1) < $unsigned(src2));  
            XOR    : aluOut = src1 ^ src2;                    
            SRL    : aluOut = src1 >> src2[4:0];
            SRA    : aluOut = $signed(src1) >>> src2[4:0];
            OR     : aluOut = src1 | src2;
            AND    : aluOut = src1 & src2;
            MUL    : aluOut = src1 * src2;
            DIV    : aluOut = src1 / src2;
            PASS   : aluOut = src2;                                             
            default: aluOut = 0;
        endcase
    end

    //Branch decision
    always_comb case (funct3[2:1])
        2'b00  : isBranchC = (funct3[0]) ^ (data_1 == data_2);                    //BNE, BEQ 
        2'b10  : isBranchC = (funct3[0]) ^ ($signed(data_1) < $signed(data_2));   //BLT, BGE
        2'b11  : isBranchC = (funct3[0]) ^ (data_1 < data_2);                     //BLTU, BGEU
        default: isBranchC = 1'b0; 
    endcase
    assign isBranchR = isBranchC & isBranch;    //isBranch is from decoding of instruction

    //Data memory
    always_ff @(posedge clock) //initial $readmemh("data/data.dat",dataMemory);
        if (isStoreW) dataMemory[aluOut] = data_2;

    assign regData = (isJALR|isJAL) ? (pc + 4) : (isLoadW ? dataMemOut : aluOut); //Writeback Mux
    assign {gp, a7, dataMemOut} = {registers[3], registers[17], dataMemory[aluOut]}; //For verification
endmodule
