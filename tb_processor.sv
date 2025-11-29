`timescale 1ns/1ps

module tb_processor;

    localparam MAX_SIM_TIME = 3000;
    localparam WIDTH = 32; 
    localparam NUM_REGS = 32;
    localparam DATA_WIDTH = 8;
    localparam MEM_DEPTH = 16384;

    logic is_timeout = 1;   
    longint sim_time = 0;
    int tests_passed = 0;
    logic [31:0] test_no = 1;

    int passed_fd, failed_fd;
    string filename;
    int tests_fd, code;
    string line;

    logic clock = 0;
    logic reset = 0;
    logic memEn;
    logic [31:0] memData;
    logic [31:0] memAddr;
    logic [31:0] gp, a7, a0;

    // Clock generator
    always #5 clock = ~clock;

    // DUT instantiation
    processor #(
        .WIDTH(WIDTH),
        .NUM_REGS(NUM_REGS),
        .DATA_WIDTH(DATA_WIDTH),
        .MEM_DEPTH(MEM_DEPTH)
    ) dut (.*);

    initial begin

        $dumpfile("waveform_tests.vcd");
        $dumpvars(5, dut);

        passed_fd = $fopen("results/passed_tests.txt", "w");
        failed_fd = $fopen("results/failed_tests.txt", "w");

        tests_fd = $fopen("tests/test_list.txt", "r");
        if (tests_fd == 0) begin
            $fatal("Cannot open test list file");
        end

        while (!$feof(tests_fd)) begin

            // Get test file to run
            $fscanf(tests_fd, "%s", line);
            filename = $sformatf("tests/%s", line);
            $display("Processing file: %s", filename);

            // Reset registers, main memory and write new program
            for (int i = 0; i < NUM_REGS; i++) dut.registers[i] = 32'h0;
            for (int i = 0; i < MEM_DEPTH; i++) dut.mainMemory[i] = 8'h00;
            $readmemh(filename, dut.mainMemory);

            sim_time = 0;
            is_timeout = 1;

            memData = 0;
            memAddr = 0;
            memEn = 0;
            reset = 1;
            #20 reset = 0;

            while ((sim_time < MAX_SIM_TIME) && is_timeout) begin

                @(posedge clock);

                //To get the number of the last test passed.
                if (gp >= 2) begin
                    test_no = gp;
                end

                // Verifying
                if (a7 == 93) begin
                    if (gp == 1) begin
                        $display("Test for file %s passed", filename);
                        $display("Last test passed: %d", test_no);
                        // Write to passed tests file
                        $fwrite(passed_fd, "%s\n", filename);
                        $fwrite(passed_fd, "Last test passed: %d\n", test_no);
                        tests_passed = tests_passed + 1;
                    end
                    else begin
                        test_no = gp >> 1;
                        $display("Test for file %s failed", filename);
                        $display("Test failed at: %d", test_no);
                        // Write to failed tests file
                        $fwrite(failed_fd, "%s\n", filename);
                        $fwrite(failed_fd, "Test failed at: %d\n", test_no);
                    end
                    is_timeout = 0;
                end

                sim_time = sim_time + 1;
            end

            if (is_timeout) begin
                test_no = gp >> 1;
                $display("Test for file %s failed", filename);
                $display("Test timed out at: %d", test_no);
                // Write to failed tests file
                $fwrite(failed_fd, "%s\n", filename);
                $fwrite(failed_fd, "Test timed out at: %d\n", test_no);
            end

            $display("");
        end

        $display("Total tests passed: %d\n", tests_passed);

        $fclose(passed_fd);
        $fclose(failed_fd);
        $fclose(tests_fd);
        
        $finish;
    end

endmodule