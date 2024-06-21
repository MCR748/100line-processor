//`default_nettype none

module processor #(
  WIDTH = 32, IMEM_DEPTH=512, DMEM_DEPTH=32, NUM_REGS=32
)(
  input  wire clock, reset, insMemEn,
  input  wire [WIDTH-1:0] insMemDataIn, insMemAddr,
  output reg  [WIDTH-1:0] gp, a7 //verifying
);
  reg [WIDTH-1:0] dataMemory [DMEM_DEPTH-1:0];
  reg [WIDTH-1:0] insMemory  [IMEM_DEPTH-1:0];
  reg [WIDTH-1:0] registers  [NUM_REGS  -1:0];
  reg [3:0] aluOp;
  reg [4:0] rs1, rs2, rd, opcode;
  reg [2:0] funct3;
  reg [6:0] funct7;
  reg [WIDTH-1:0] ins, imm, pc, data_1, data_2, regDataIn, src1, src2, aluOut, dataMemOut;
  reg isArithmetic, isImm, isLoadW, isLoadUI, isStoreW, isBranch, isJAL, isJALR, isMUL, isAUIPC, isBranchC, regWriteEn;
  
  //PC
  always @(posedge clock)
    if (reset) pc <= 0;
    else       pc <= (isJAL|isJALR|isBranchC) ? aluOut : (pc + 4);

  //Instruction memory    //initial $readmemh("tests/rv32ui-p-lui.dump.dat", insMemory);
  always @(posedge clock) 
    if (insMemEn) insMemory[insMemAddr] <= insMemDataIn;

  always @* begin

    ins = insMemEn ? 32'h13 : insMemory[pc[10:2]];

    //Instruction decoder
    {funct7, rs2, rs1, funct3, rd, opcode} = ins[31:2];
    
    isArithmetic = (opcode == 5'b01100) & (funct7[0] == 1'b0);
    isMUL        = (opcode == 5'b01100) & (funct7[0] == 1'b1); //For MUL and DIV
    isImm        = (opcode == 5'b00100);
    isLoadW      = (opcode == 5'b00000);
    isLoadUI     = (opcode == 5'b01101);
    isStoreW     = (opcode == 5'b01000);
    isBranch     = (opcode == 5'b11000);
    isJAL        = (opcode == 5'b11011);
    isJALR       = (opcode == 5'b11001);
    isAUIPC      = (opcode == 5'b00101);

    // Immediate generation
    if (isImm|isLoadW|isJALR)  imm = WIDTH'($signed( ins[31:20]));                                            //iImm
    else if (isLoadUI|isAUIPC) imm = {ins[31:12], 12'b0};                                                     //uImm
    else if (isStoreW)         imm = WIDTH'($signed({ins[31:25], ins[11:7]}));                                //sImm
    else if (isBranch)         imm = WIDTH'($signed({ins[31]   , ins[7]    , ins[30:25], ins[11:8] , 1'b0})); //sbImm
    else if (isJAL)            imm = WIDTH'($signed({ins[31]   , ins[19:12], ins[20]   , ins[30:21], 1'b0})); //jImm
    else                       imm = 0;

    //Read Registers     
    data_1 = (rs1 == 0) ? 0 : registers[rs1];
    data_2 = (rs2 == 0) ? 0 : registers[rs2];

    //Branch decision
    case (funct3[2:1])
      2'b00  : isBranchC = isBranch & (funct3[0] ^ (data_1 == data_2));                  //BNE, BEQ 
      2'b10  : isBranchC = isBranch & (funct3[0] ^ ($signed(data_1) < $signed(data_2))); //BLT, BGE
      2'b11  : isBranchC = isBranch & (funct3[0] ^ (data_1 < data_2));                   //BLTU, BGEU
      default: isBranchC = 1'b0;
    endcase
  end  

  //ALU
  localparam [3:0] ADD=0, SLL=1, SLT=2, SLTU=3, XOR=4, SRL=5, OR=6, AND=7, SUB=8, MUL=9, DIV=10, SRA=13, PASS=15;

  always @* begin
    if      (isMUL)                                          aluOp = (funct3[2] ? DIV : MUL);
    else if (isArithmetic)                                   aluOp = {funct7[5]                   , funct3};
    else if (isImm)                                          aluOp = {funct7[5] & (funct3==3'b101), funct3};
    else if (isAUIPC|isJAL|isJALR|isBranch|isLoadW|isStoreW) aluOp = ADD ;   //Can put with load and store
    else                                                     aluOp = PASS;

    src1 = (isJAL|isBranch|isAUIPC)                                        ? pc  : data_1;
    src2 = (isImm|isLoadW|isLoadUI|isJAL|isJALR|isStoreW|isBranch|isAUIPC) ? imm : data_2;

    case (aluOp)
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

    {gp, a7, dataMemOut} = {registers[3], registers[17], dataMemory[aluOut]}; //For verification

    // Writeback to register bank
    regWriteEn = isArithmetic|isImm|isLoadW|isLoadUI|isJAL|isJALR|isAUIPC;
    regDataIn  = (isJALR|isJAL) ? (pc + 4) : (isLoadW ? dataMemOut : aluOut); //Writeback Mux
  end

  //Data memory    //initial $readmemh("data/data.dat",dataMemory);
  always @(posedge clock) 
    if (isStoreW) dataMemory[aluOut] <= data_2;

  always @(posedge clock) //initial $readmemh("data/registry.dat", registers);
    if (regWriteEn) registers[rd] <= regDataIn;

endmodule
