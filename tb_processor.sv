`include "processor.sv"

class Random_Num #(WIDTH=8);
    rand bit signed [WIDTH-1:0] num;
endclass

module tb_processor;
    timeunit 1ns;
    timeprecision 1ps;
    parameter WIDTH = 32;
    //Module IO
    logic clock = 0;
    logic reset = 0;
    logic result;
    processor #(.WIDTH(WIDTH)) dut (.*);

    //Testbench variables
    bit [WIDTH-1:0] pc, pc_next, testing_immediate, prev_pc;
    bit [WIDTH-1:0] register_file [0:31];
    bit [WIDTH-1:0] testing_instruction, prev_testing_instruction;
    bit [4:0] testing_rs1, testing_rs2, testing_rd;
    Random_Num #(.WIDTH(WIDTH)) inital_reg_val = new();
    
    localparam CLK_PERIOD = 10;
    initial forever #(CLK_PERIOD/2) clock <= ~clock;

    initial begin
        //Initializing the register_file values
        //Keep the copy of the values assigned in an array - to use in comparison 
        for (int i = 0; i < 32 ; i++) begin
            @(posedge clock);
            inital_reg_val.randomize();
            register_file[i] = inital_reg_val.num;
            dut.registers[i] = inital_reg_val.num;
        end 
       
        //To start the processor
        @(posedge clock);
        #1 reset = 1;
        @(posedge clock);
        #1 reset = 0;

        //Run the emulation here
        for (int i = 0; i < 10000 ; i++) begin
            pc = i*4;
            //testing_instruction = dut.instructionMemory[pc[10:2]];
            testing_instruction = instr_constr({$random}%33);
            @(posedge clock);
            dut.instruction = testing_instruction;
            result = instr_execute(prev_testing_instruction, register_file, prev_pc ,pc_next);
            if (result) begin
                $error("The instruction %0x failed", prev_testing_instruction);
                break;
            end

            testing_rs1 = testing_instruction[19:15];
            testing_rs2 = testing_instruction[24:20];
            testing_rd = testing_rd;
            case (testing_instruction[6:2])
                5'b01100: begin     //Arithmetic
                    pc_next = pc + 32'd4;
                    if (testing_instruction[25]) begin //MUL and DIV
                        register_file[testing_rd] = (testing_instruction[14:12] == 3'b000 ? testing_rs1 * testing_rs2 : register_file[testing_rd]);                    
                        register_file[testing_rd] = (testing_instruction[14:12] == 3'b100 ? testing_rs1 / testing_rs2 : register_file[testing_rd]);
                    end else begin
                        case ({testing_instruction[30],testing_instruction[14:12]})
                            4'b0000 : register_file[testing_rd] = testing_rs1 + testing_rs2;  //Add
                            4'b1000 : register_file[testing_rd] = testing_rs1 - testing_rs2;  //Sub
                            4'b0001 : register_file[testing_rd] = testing_rs1 << testing_rs2[4:0];  //Sll
                            4'b0010 : register_file[testing_rd] = ($signed(testing_rs1) < $signed(testing_rs2) ? 32'd1 : 32'd0);  //Slt
                            4'b0011 : register_file[testing_rd] = (testing_rs1 < testing_rs2) ? 32'd1 : 32'd0;  //Sltu
                            4'b0100 : register_file[testing_rd] = testing_rs1 ^ testing_rs2;  //Xor
                            4'b0101 : register_file[testing_rd] = testing_rs1 >> testing_rs2[4:0];  //Srl
                            4'b1101 : register_file[testing_rd] =  $signed(testing_rs1) >>> testing_rs2[4:0];  //Sra
                            4'b0110 : register_file[testing_rd] = testing_rs1 | testing_rs2;  //Or
                            4'b0111 : register_file[testing_rd] = testing_rs1 & testing_rs2;  //And 
                            default: register_file[testing_rd] = register_file[testing_rd];
                        endcase
                    end
                end 
                5'b00100: begin     //Immediate
                    pc_next = pc + 32'd4;
                    testing_immediate = {{21{testing_instruction[31]}}, testing_instruction[30:20]};
                    case (testing_instruction[14:12])
                        3'b000 : register_file[testing_rd] = testing_immediate + testing_rs2;  //Add
                        3'b001 : register_file[testing_rd] = testing_immediate << testing_rs2[4:0];  //Sll
                        3'b010 : register_file[testing_rd] = ($signed(testing_immediate) < $signed(testing_rs2) ? 32'd1 : 32'd0);  //Slt
                        3'b011 : register_file[testing_rd] = (testing_immediate < testing_rs2) ? 32'd1 : 32'd0;  //Sltu
                        3'b100 : register_file[testing_rd] = testing_immediate ^ testing_rs2;  //Xor
                        3'b101 : register_file[testing_rd] = testing_instruction[30] ? $signed(testing_immediate) >>> testing_rs2[4:0] : testing_immediate >> testing_rs2[4:0];  //Srl
                        3'b110 : register_file[testing_rd] = testing_immediate | testing_rs2;  //Or
                        3'b111 : register_file[testing_rd] = testing_immediate & testing_rs2;  //And 
                        default: register_file[testing_rd] = register_file[testing_rd];
                    endcase
                end
                5'b00000 : begin    //Load
                    pc_next = pc + 32'd4;
                    testing_immediate = {{21{testing_instruction[31]}}, testing_instruction[30:20]};
                    if (testing_instruction[14:12] == 3'b010) begin
                        register_file[testing_instruction[11:7]] = {dut.dataMemory[testing_immediate + register_file[testing_instruction[19:15]]],
                                                                dut.dataMemory[testing_immediate + register_file[testing_instruction[19:15]] + 1],
                                                                dut.dataMemory[testing_immediate + register_file[testing_instruction[19:15]] + 2],
                                                                dut.dataMemory[testing_immediate + register_file[testing_instruction[19:15]] + 3]};
                    end
                end 
                5'b01101 : begin    //LUI
                    pc_next = pc + 32'd4;
                    testing_immediate = {testing_instruction[31:12],12'b0}; 
                    register_file[testing_rd] = testing_immediate;  
                end 
                5'b01000 : begin    //Store
                    pc_next = pc + 32'd4;
                    testing_immediate = {{21{testing_instruction[31]}},testing_instruction[30:25],testing_instruction[11:7]};  
                end 
                5'b11000 : begin    //Branch
                    testing_immediate = {{20{testing_instruction[31]}},testing_instruction[7],testing_instruction[30:25],testing_instruction[11:8],1'b0};  
                    case (testing_instruction[14:12])
                        3'b000: pc_next = (register_file[testing_instruction[19:15]] == register_file[testing_instruction[24:20]]) ? pc + testing_immediate : pc + 4;
                        3'b001: pc_next = (register_file[testing_instruction[19:15]] != register_file[testing_instruction[24:20]]) ? pc + testing_immediate : pc + 4; 
                        3'b100: pc_next = (register_file[testing_instruction[19:15]] < register_file[testing_instruction[24:20]]) ? pc + testing_immediate : pc + 4;
                        3'b101: pc_next = (register_file[testing_instruction[19:15]] >= register_file[testing_instruction[24:20]]) ? pc + testing_immediate : pc + 4;
                        3'b110: pc_next = ($signed(register_file[testing_instruction[19:15]]) < $signed(register_file[testing_instruction[24:20]])) ? pc + testing_immediate : pc + 4;
                        3'b111: pc_next = ($signed(register_file[testing_instruction[19:15]]) >= $signed(register_file[testing_instruction[24:20]])) ? pc + testing_immediate : pc + 4;
                        default: pc_next = pc + 4;
                    endcase     
                end 
                5'b11011 : begin    //JAL
                    register_file[testing_instruction[11:7]] = pc + 32'd4;
                    testing_immediate = {{12{testing_instruction[31]}}, testing_instruction[19:12], testing_instruction[20], testing_instruction[30:21], 1'b0};
                    pc_next = pc + testing_immediate;
                end 
                5'b11001 : begin    //JALR
                    register_file[testing_instruction[11:7]] = pc + 32'd4;
                    testing_immediate = {testing_instruction[31:12],12'b0}; 
                    pc_next = register_file[testing_instruction[19:15]]  + testing_immediate;
                end 
                5'b00101 : begin    //AUIPC
                    pc_next = pc + 32'd4;
                    testing_immediate = {{12{testing_instruction[31]}}, testing_instruction[19:12], testing_instruction[20], testing_instruction[30:21], 1'b0};
                    register_file[testing_instruction[11:7]] = pc + testing_immediate;

                end 
                default: register_file[testing_rd] = register_file[testing_rd];
            endcase
            prev_testing_instruction = testing_instruction;
            prev_pc = pc;

        end
        $finish;
    end

    //Setting a function to provide a given testing_instruction according to the number given in
    function automatic bit [WIDTH-1:0] instr_constr;
        input integer i;
        bit [WIDTH-1:0] testing_instruction [0:33];
        bit [WIDTH-1:0] return_instruction;
        //Define the testing_instruction array
        //In the order of Arithmetic, Immediate, LoadUI, Load,
        // Store, Branch, Jal, Jalr, AUIPC
        bit [4:0] opcode [0:8] = '{01100,00100,01101,00000,01000,11000,11011,11001,00101};
        bit [12:0] s_imm;

        //Getting the random values
        Random_Num #(.WIDTH(5)) shamt = new();
        Random_Num #(.WIDTH(5)) rs1 = new(), rs2 = new(), rd = new();
        Random_Num #(.WIDTH(12)) i_imm = new();     //For I, S and B
        Random_Num #(.WIDTH(20)) u_imm = new();     //For U and J
        rs1.randomize();
        rs2.randomize();
        rd.randomize();
        shamt.randomize;
        i_imm.randomize();
        u_imm.randomize();
        s_imm= {{i_imm.num},1'b0};

        //Instructions numbered in the order of the source document numbering
        testing_instruction[0] = 32'h00000013;              //NOP

        testing_instruction[1]  = {7'b00000_00,{rs2.num},{rs1.num},3'b000,{rd.num},{opcode[0]},2'b11};   //Add
        testing_instruction[2]  = {7'b10000_00,{rs2.num},{rs1.num},3'b000,{rd.num},{opcode[0]},2'b11};   //Sub
        testing_instruction[3]  = {7'b00000_00,{rs2.num},{rs1.num},3'b001,{rd.num},{opcode[0]},2'b11};   //Sll
        testing_instruction[4]  = {7'b00000_00,{rs2.num},{rs1.num},3'b010,{rd.num},{opcode[0]},2'b11};   //Slt
        testing_instruction[5]  = {7'b00000_00,{rs2.num},{rs1.num},3'b011,{rd.num},{opcode[0]},2'b11};   //Sltu
        testing_instruction[6]  = {7'b00000_00,{rs2.num},{rs1.num},3'b100,{rd.num},{opcode[0]},2'b11};   //Xor
        testing_instruction[7]  = {7'b00000_00,{rs2.num},{rs1.num},3'b101,{rd.num},{opcode[0]},2'b11};   //Srl
        testing_instruction[8]  = {7'b01000_00,{rs2.num},{rs1.num},3'b101,{rd.num},{opcode[0]},2'b11};   //Sra
        testing_instruction[9]  = {7'b00000_00,{rs2.num},{rs1.num},3'b110,{rd.num},{opcode[0]},2'b11};   //Or
        testing_instruction[10] = {7'b00000_00,{rs2.num},{rs1.num},3'b111,{rd.num},{opcode[0]},2'b11};  //And
        testing_instruction[11] = {7'b00000_01,{rs2.num},{rs1.num},3'b000,{rd.num},{opcode[0]},2'b11};  //Mul
        testing_instruction[12] = {7'b00000_01,{rs2.num},{rs1.num},3'b100,{rd.num},{opcode[0]},2'b11};  //Div

        testing_instruction[13] = {{i_imm.num}            ,{rs1.num},3'b000,{rd.num},{opcode[1]},2'b11};   //Addi
        testing_instruction[14] = {7'b00000_00,{shamt.num},{rs1.num},3'b001,{rd.num},{opcode[1]},2'b11};   //Slli
        testing_instruction[15] = {{i_imm.num}            ,{rs1.num},3'b010,{rd.num},{opcode[1]},2'b11};   //Slti
        testing_instruction[16] = {{i_imm.num}            ,{rs1.num},3'b011,{rd.num},{opcode[1]},2'b11};   //Sltui
        testing_instruction[17] = {{i_imm.num}            ,{rs1.num},3'b100,{rd.num},{opcode[1]},2'b11};   //Xori
        testing_instruction[18] = {7'b00000_00,{shamt.num},{rs1.num},3'b101,{rd.num},{opcode[1]},2'b11};   //Srli
        testing_instruction[19] = {7'b01000_00,{shamt.num},{rs1.num},3'b101,{rd.num},{opcode[1]},2'b11};   //Srai
        testing_instruction[20] = {{i_imm.num}            ,{rs1.num},3'b110,{rd.num},{opcode[1]},2'b11};   //Ori
        testing_instruction[21] = {{i_imm.num}            ,{rs1.num},3'b111,{rd.num},{opcode[1]},2'b11};  //Andi
        
        testing_instruction[22] = {{u_imm.num},                 {rd.num},{opcode[2]},2'b11};    //LUI
        testing_instruction[23] = {{i_imm.num},{rs1.num},3'b010,{rd.num},{opcode[3]},2'b11};    //LW

        testing_instruction[24] = {{{i_imm.num[11:5]}},{rs2.num},{rs1.num},3'b010,{{i_imm.num[4:0]}},{opcode[4]},2'b11};   //SW

        testing_instruction[25] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b000,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Beq
        testing_instruction[26] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b001,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Bne
        testing_instruction[27] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b100,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Blt
        testing_instruction[28] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b101,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Bge
        testing_instruction[29] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b110,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Bltu
        testing_instruction[30] = {{{s_imm[12]}},{{s_imm[10:5]}},{rs2.num},{rs1.num},3'b111,{{s_imm[4:1]}},{{s_imm[11]}},{opcode[5]},2'b11};  //Bgeu

        testing_instruction[31] = {{{u_imm.num[20]}},{{u_imm.num[10:1]}},{{u_imm.num[11]}},{{u_imm.num[19:12]}},{rd.num},{opcode[6]},2'b11}; //Jal
        testing_instruction[32] = {{i_imm.num},{rs1.num},3'b000,{rd.num},{opcode[7]},2'b11};   //Jalr
        testing_instruction[33] = {{u_imm.num},                 {rd.num},{opcode[2]},2'b11};   //AUIPC

        // instr_constr = testing_instruction[i];
        return_instruction = testing_instruction[i];
        return return_instruction;
    endfunction
    

    //Setting a function to verify the operation
    //Will give the output 1'b1 if testing_instruction verified, else 1'b0
    function automatic bit instr_execute;
        input bit [WIDTH-1:0] instr;
        input bit [WIDTH-1:0] registers [0:31];     //Pass dut.registerfile
        input bit [WIDTH-1:0] testing_pc;           //Pass dut.pc
        input bit [WIDTH-1:0] testing_pc_next;           //Pass dut.pc_next
        //Define the testing_instruction array
        //In the order of Arithmetic, Immediate, LoadUI, Load,
        // Store, Branch, Jal, Jalr, AUIPC
        bit [4:0] opcode [0:8] = '{01100,00100,01101,00000,01000,11000,11011,11001,00101};
        bit return_value = 1'b0;
        //Defining required varaibles
        bit [4:0] testing_opcode = instr[6:2];
        bit [5:0] rs1            = instr[19:15];
        bit [5:0] rs2            = instr[24:20];
        bit [5:0] rd             = instr[11:7];
        bit [2:0] funct3         = instr[14:12];
        bit [6:0] funct7         = instr[31:27];
        bit [31:0] i_imm         = {{21{instr[31]}}, instr[30:20]};                                 
        bit [31:0] s_imm         = {{21{instr[31]}},instr[30:25],instr[11:7]};                      
        bit [31:0] b_imm         = {{20{instr[31]}},instr[7],instr[30:25],instr[11:8],1'b0};       
        bit [31:0] u_imm         = {instr[31:12],12'b0};                                            
        bit [31:0] j_imm         = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};

        
        if (testing_opcode == opcode[0]) begin                      //Arithmetic and Mul and Div
            return_value = (registers[rd] == register_file[rd]);
        end else if (testing_opcode == opcode[1]) begin            //Arithmetic with Immediate
            return_value = (registers[rd] == register_file[rd]);
        end else if (testing_opcode == opcode[2]) begin            //LoadUI
            return_value = (registers[rd] == register_file[rd]);
        end else if (testing_opcode == opcode[3]) begin            //Load Word
            return_value = (registers[rd] == register_file[rd]) && funct3 == 3'b010;
        end else if (testing_opcode == opcode[4]) begin            //Store Word
            return_value = (funct3 = 3'b010 && (register_file[rd] ==
                            {dut.dataMemory[s_imm + register_file[rs1]],
                            dut.dataMemory[s_imm + register_file[rs1] + 1],
                            dut.dataMemory[s_imm + register_file[rs1] + 2],
                            dut.dataMemory[s_imm + register_file[rs1] + 3]}));
        end else if (testing_opcode == opcode[5]) begin            //Branch
            return_value = (testing_pc_next == pc_next);
        end else if (testing_opcode == opcode[6]) begin            //Jal
            return_value = (registers[rd] == register_file[rd]) && (register_file[rd] == pc + 3'd4) && (testing_pc_next == pc + j_imm);
        end else if (testing_opcode == opcode[7]) begin            //Jalr
            return_value = (registers[rd] == register_file[rd]) && (register_file[rd] == pc + 3'd4) && (testing_pc_next == pc + registers[rs1]);
        end else if (testing_opcode == opcode[8]) begin            //AUIPC
            return_value = (register_file[rd] == registers[rd]) && (register_file[rd] == pc + u_imm);
        end else begin
            //Invalid testing_instruction
            return_value = 1'b0;
        end

        return return_value;
    endfunction

    
endmodule
