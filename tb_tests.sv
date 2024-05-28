`timescale 1ns/1ps

module tb_tests;
    parameter WIDTH = 32;

    //Module IO
    logic clock = 0;
    logic reset = 0;
    logic result;

    processor #(.WIDTH(WIDTH)) dut (.*);

    localparam CLK_PERIOD = 10;
    initial forever #(CLK_PERIOD/2) clock <= ~clock;

    int file, status, total_tests = 0;
    string line;
    bit finish, local_passed;

    function automatic testing;
        input string file_name; 
        $readmemh({"./tests/",file_name},dut.instructionMemory);
        
        finish = 0;
        local_passed = 0;
        while (!finish) begin
            if (dut.registers[17] == 32'd93) begin
                finish = 1;
                if (dut.registers[3] == 32'b1) begin
                    local_passed = 1;
                    $display("Test %s passed", file_name);
                end else begin
                    $display("Test %s failed", file_name);
                end
            end
        end
        return local_passed;
    endfunction



    initial begin
    file = $fopen("./file_list.txt", "r");
    if (file == 0) begin
      $display("Failed to open file_list.txt");
      $finish;
    end

    while (!$feof(file)) begin
      status = $fgets(line, file);
      if (status!= 0) begin
        $display("Read file name: %s", line);

        #10 reset = 1;
        #10 reset = 0;

        if (testing(line)) begin
            total_tests += 0;
        end         
      end else begin
        $display("Error reading file_list.txt");
      end
    end

    $display("Total tests passed: %d", total_tests);

    $fclose(file);
  end

    
endmodule