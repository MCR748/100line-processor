//`default_nettype none

module processor #(
  WIDTH = 32, NUM_REGS=32, DATA_WIDTH = 8, MEM_DEPTH=16384
) (
  input logic clock, reset, memEn,
  input logic [WIDTH-1: 0] memData, 
  input logic [WIDTH-1: 0]memAddr,
  output logic [WIDTH-1: 0] gp, a7, a0
);
  logic [DATA_WIDTH-1:0] mainMemory [0 : MEM_DEPTH-1];
  logic [WIDTH-1:0] registers  [0 : NUM_REGS  -1];
  logic [3:0] aluOp;
  logic [4:0] rs1, rs2, rd, opcode;
  logic [2:0] funct3;
  logic [6:0] funct7;
  logic [WIDTH-1:0] ins, imm, pc, data_1, data_2, regDataIn, src1, src2, aluOut, memRead, memWrite;
  logic isArithmetic, isImm, isMemRead, isLoadUI, isMemWrite, isBranch, isJAL, isJALR, isMUL, isAUIPC, isBranchR, isBranchC, regWriteEn;

  //PC
  always_ff @(posedge clock)
    if (reset) pc <= 0;
    else       pc <= (isJAL|isJALR|isBranchR) ? aluOut : pc + 4;

  //Instruction memory    //initial $readmemh("tests/rv32ui-p-lui.dump.dat", mainMemory);
  always_ff @(posedge clock) 
      mainMemory[memAddr[13:0]] <= (memEn) ? memData[7:0] : mainMemory[memAddr[13:0]] ;

  assign ins = (~memEn) ? {mainMemory[pc+3], mainMemory[pc+2], mainMemory[pc+1], mainMemory[pc]} : 32'h00000013;

 always_comb begin
    //Instruction decoder
    {funct7, rs2, rs1, funct3, rd, opcode} = ins[31:2];
    
    isArithmetic = (opcode == 5'b01100) & (funct7[0] == 1'b0);
    isMUL        = (opcode == 5'b01100) & (funct7[0] == 1'b1); //For MUL and DIV
    isImm        = (opcode == 5'b00100);
    isMemRead    = (opcode == 5'b00000);
    isLoadUI     = (opcode == 5'b01101);
    isMemWrite   = (opcode == 5'b01000);
    isBranch     = (opcode == 5'b11000);
    isJAL        = (opcode == 5'b11011);
    isJALR       = (opcode == 5'b11001);
    isAUIPC      = (opcode == 5'b00101);

    // Immediate generation
    if (isImm|isMemRead|isJALR)   imm = WIDTH'($signed( ins[31:20]));                                            //iImm
    else if (isLoadUI|isAUIPC)    imm = {ins[31:12], 12'b0};                                                     //uImm
    else if (isMemWrite)          imm = WIDTH'($signed({ins[31:25], ins[11:7]}));                                //sImm
    else if (isBranch)            imm = WIDTH'($signed({ins[31]   , ins[7]    , ins[30:25], ins[11:8] , 1'b0})); //sbImm
    else if (isJAL)               imm = WIDTH'($signed({ins[31]   , ins[19:12], ins[20]   , ins[30:21], 1'b0})); //jImm
    else                          imm = 0;
  end

  //Registry    //initial $readmemh("data/registry.dat", registers);
  assign data_1 = (rs1 == 0) ? 0 : registers[rs1];
  assign data_2 = (rs2 == 0) ? 0 : registers[rs2];

  //ALU
  localparam [3:0] ADD=0, SLL=1, SLT=2, SLTU=3, XOR=4, SRL=5, OR=6, AND=7, SUB=8, MUL=9, DIV=10, SRA=13, PASS=15;

  always_comb begin
    if      (isMUL)                                               aluOp = (funct3[2] ? DIV : MUL);
    else if (isArithmetic)                                        aluOp = {funct7[5]                   , funct3};
    else if (isImm)                                               aluOp = {funct7[5] & (funct3==3'b101), funct3};
    else if (isAUIPC|isJAL|isJALR|isBranch|isMemRead|isMemWrite)  aluOp = ADD ;   //Can put with load and store
    else                                                          aluOp = PASS;

    src1 = (isJAL|isBranch|isAUIPC)                                             ? pc  : data_1;
    src2 = (isImm|isMemRead|isLoadUI|isJAL|isJALR|isMemWrite|isBranch|isAUIPC)  ? imm : data_2;

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
      DIV    : aluOut = ($signed(src2) == 0) ? 32'hFFFFFFFF : (($signed(src1) == 32'h80000000 && $signed(src2) == -1) ? 32'h80000000 : WIDTH'($signed(src1) / $signed(src2)));
      PASS   : aluOut = src2;                                             
      default: aluOut = 0;
    endcase 
  end

    //Branch decision
    always_comb begin : BranchComparator
        case (funct3[2:1])
            2'b00: isBranchC = (funct3[0]) ^ (data_1 == data_2);                    //BNE, BEQ 
            2'b10: isBranchC = (funct3[0]) ^ ($signed(data_1) < $signed(data_2));   //BLT, BGE
            2'b11: isBranchC = (funct3[0]) ^ (data_1 < data_2);                     //BLTU, BGEU
            default: isBranchC = 1'b0; 
        endcase
      isBranchR = isBranchC & isBranch;
    end
  
  // Store data assign
  always_comb begin : MemWriteAssign
    case (funct3)
      3'b000: memWrite  = {24'b0,data_2[7:0]};                                      //SB
      3'b001: memWrite  = {16'b0, data_2[15:8],data_2[7:0]};                        //SHW
      3'b010: memWrite  = {data_2[31:24], data_2[23:16], data_2[15:8],data_2[7:0]}; //SW
      default: memWrite = 32'b0;
    endcase
  end
    //Data memory    //initial $readmemh("data/data.dat",mainMemory);
  always_ff @(posedge clock) begin : DataMemoryUpdate
      if (isMemWrite) begin
          case (funct3)
            3'b000:        //SB                                                                
              mainMemory[aluOut]   <= memWrite[7:0];
            3'b001: begin //SHW
              mainMemory[aluOut]   <= memWrite[7:0];
              mainMemory[aluOut+1] <= memWrite[15:8];
            end                                                                   
            3'b010: begin //SW
              mainMemory[aluOut]   <= memWrite[7:0];
              mainMemory[aluOut+1] <= memWrite[15:8];
              mainMemory[aluOut+2] <= memWrite[23:16];
              mainMemory[aluOut+3] <= memWrite[31:24];
            end
            default: begin
              mainMemory[aluOut]   <= mainMemory[aluOut];
              mainMemory[aluOut+1] <= mainMemory[aluOut+1];
              mainMemory[aluOut+2] <= mainMemory[aluOut+2];
              mainMemory[aluOut+3] <= mainMemory[aluOut+3];
            end                                                                  
          endcase
      end
    end

  //Read Data
  always_comb begin : MemRead
    case (funct3[1:0])
      2'b00: memRead = (!funct3[2]) ? ({{24{mainMemory[aluOut][7]}}, mainMemory[aluOut]}) : ({24'b0, mainMemory[aluOut]});
      2'b01: memRead = (!funct3[2]) ? ({{16{mainMemory[aluOut+1][7]}}, {mainMemory[aluOut+1], mainMemory[aluOut]}}) : ({16'b0, {mainMemory[aluOut+1], mainMemory[aluOut]}});
      2'b10: memRead = {mainMemory[aluOut+3], mainMemory[aluOut+2], mainMemory[aluOut+1], mainMemory[aluOut]};
      default: memRead = 32'b0;
    endcase
  end

  // Writeback to register bank
  assign regWriteEn = isArithmetic|isImm|isMemRead|isLoadUI|isJAL|isJALR|isAUIPC|isMUL;
  assign regDataIn  = (isJALR|isJAL) ? (pc + 4) : (isMemRead ? memRead : aluOut); //Writeback Mux

  always_ff @(posedge clock) //initial $readmemh("data/registry.dat", registers);
    if (regWriteEn) registers[rd] <= regDataIn;

  //verifying
  assign {gp, a7, a0} = {registers[3], registers[17], registers[10]}; //For verification 
endmodule