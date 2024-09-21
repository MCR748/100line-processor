`timescale 1ns/1ps

module tb_tests;

    //Module IO
    logic clock = 0;
    logic reset = 1;
	reg insMemEn = 0;
	reg [31:0] insMemDataIn, insMemAddr;

    processor dut (.clock(clock) , .reset(reset) , .insMemEn(insMemEn) , .insMemAddr(insMemAddr) , .insMemDataIn(insMemDataIn));

    localparam CLK_PERIOD = 10;
    initial forever #(CLK_PERIOD/2) clock <= ~clock;

    string file_name;
    reg finished = 0;
	int i = 0;
	int j = 0;
	int done = 0;

	reg [31:0] sam [511:0];
	reg [31:0] sam2 [31:0];
	reg [31:0] file [511:0];

	initial begin
		dut.dataMemory = sam2;
		$readmemh("C:/Users/Jazoolee/Desktop/Quartus/processor_verilog/data/data.dat",dut.dataMemory);

		 while (i <= 32) begin
			@(posedge clock);
			dut.dataMemory = sam2;
			dut.insMemory = sam;
			case (i) 
				0 : file_name = "rv32ui-p-add.dump.dat";
				1 : file_name = "rv32ui-p-addi.dump.dat";
				2 : file_name = "rv32ui-p-and.dump.dat";
				3 : file_name = "rv32ui-p-andi.dump.dat";
				4 : file_name = "rv32ui-p-auipc.dump.dat";
				5 : file_name = "rv32ui-p-beq.dump.dat";
				6 : file_name = "rv32ui-p-bge.dump.dat";
				7 : file_name = "rv32ui-p-bgeu.dump.dat";
				8 : file_name = "rv32ui-p-blt.dump.dat";
				9 : file_name = "rv32ui-p-bltu.dump.dat";
				10 : file_name = "rv32ui-p-bne.dump.dat";
				11 : file_name = "rv32ui-p-jal.dump.dat";
				12 : file_name = "rv32ui-p-jalr.dump.dat";
				13: file_name = "rv32ui-p-lui.dump.dat";
				14: file_name = "rv32ui-p-add.dump.dat"; //"rv32ui-p-lw.dump.dat"; //
				15: file_name = "rv32ui-p-or.dump.dat";
				16: file_name = "rv32ui-p-ori.dump.dat";
				17: file_name = "rv32ui-p-sll.dump.dat";
				18: file_name = "rv32ui-p-slli.dump.dat";
				19: file_name = "rv32ui-p-slt.dump.dat";
				20: file_name = "rv32ui-p-slti.dump.dat";
				21: file_name = "rv32ui-p-sltiu.dump.dat";
				22: file_name = "rv32ui-p-sltu.dump.dat";
				23: file_name = "rv32ui-p-sra.dump.dat";
				24: file_name = "rv32ui-p-srai.dump.dat";
				25: file_name = "rv32ui-p-srl.dump.dat";
				26: file_name = "rv32ui-p-srli.dump.dat";
				27: file_name = "rv32ui-p-sub.dump.dat";
				28: file_name = "rv32ui-p-sw.dump.dat";
				29: file_name = "rv32ui-p-xor.dump.dat";
				30: file_name = "rv32ui-p-xori.dump.dat";//"rv32ui-p-lw.dump.dat"; //
				31: file_name = "rv32ui-p-sb.dump.dat";
				32: file_name = "rv32ui-p-sh.dump.dat";
			endcase

			
			$readmemh({"C:/Users/Jazoolee/Desktop/Quartus/processor_verilog/tests/", file_name },file);
			i = i + 1;
			j = 0;
			done = 0;

			while (j <= 511) begin
				@(posedge clock);

				insMemAddr = j;
				if (file[j] === 32'b0) begin
					done = 1;
					break;
				end

				insMemDataIn = file[j];
				insMemEn = 1;

				j = j + 1;

			end

			if (j == 512 | done) begin
				@(negedge clock);

				#2 reset = 0;
				insMemEn = 0;
				finished = 0;
				$display("Started Testing %s" , file_name);
				while (!finished) begin
					@(negedge clock);

					if (dut.processor_only_inst.registers[17] == 32'd93) begin
						finished = 1;
						reset = 1;
						$display("Finished !");
						dut.processor_only_inst.registers[17] = 32'bx;
							if (dut.processor_only_inst.registers[3] == 32'b1) begin
								$display("Test %s passed", file_name);
								dut.processor_only_inst.registers[3] = 32'bx;
							end else begin
									$display("Test %s failed", file_name);
								end
					end
				end
			end

			if (i == 33) $finish;

		end
	end
   
endmodule