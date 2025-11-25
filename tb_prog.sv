`timescale 1ns/1ps

module tb_prog;

    localparam MAX_SIM_TIME = 100000;
    localparam DIN_ADDR = 'h3E00;
    localparam DOUT_ADDR = 'h3F00;
    localparam MAX_IO_SIZE = 256;   // Bytes

    localparam WIDTH = 32; 
    localparam NUM_REGS = 32;
    localparam DATA_WIDTH = 8;
    localparam MEM_DEPTH = 16384;

    enum logic [2:0] {
        INIT,
        SEND_INST,
        SEND_DATA,
        RESET, 
        RUN
    } sim_state;

    logic is_timeout = 1;   
    longint sim_time = 0;
    logic [31:0] data_idx = 0;
    int numOutputs = 0;

    logic clock = 0;
    logic reset = 0;
    logic memEn;
    logic [31:0] memData;
    logic [31:0] memAddr;
    logic [31:0] gp, a7, a0;

    logic [7:0] inputBytes [0:MAX_IO_SIZE-1];
    logic [7:0] outputBytes [0:MAX_IO_SIZE-1];
    int inputSize = 0;

    task automatic readInputData(input string filename);
        int fd;
        string line;
        longint temp;    // parse into 64-bit first (like C++ stoll)
        int scanned;
        int value;
        int tmp;
        int idx = 0;

        // --- Open file ---
        fd = $fopen(filename, "r");
        if (fd == 0)
        $fatal(1, "ERROR: Cannot open input file %s", filename);

        // --- Read line-by-line ---
        while (!$feof(fd)) begin
            line = "";
            void'($fgets(line, fd));

            // Skip empty or whitespace-only lines
            if (line.len() == 0) continue;

            // Try to parse decimal integer (full token)
            scanned = $sscanf(line, "%d", temp);

            if (scanned != 1) begin
                $error("Invalid integer: \"%s\"", line);
                continue;
            end

            // Safe cast to 32-bit signed
            value = int'(temp);

            // --- Split into 4 bytes (little-endian) ---
            tmp = value;
            for (int i = 0; i < 4; i++) begin
                if (idx >= MAX_IO_SIZE) begin
                    $warning("Input exceeds MAX_IO_SIZE (%0d). Remaining bytes discarded.", MAX_IO_SIZE);
                    break;   // exit task
                end

                inputBytes[idx] = tmp[7:0];
                tmp = tmp >> 8;
                idx++;
            end
        end
        inputSize = idx;

        $fclose(fd);
    endtask

    task automatic writeOutputData(
        input string filename,
        input int numOutputsWords    // number of 32-bit words
    );
        int fd;
        int numBytes;
        logic [31:0] value;
        int idx;

        // --- open file ---
        fd = $fopen(filename, "w");
        if (fd == 0)
        $fatal(1, "ERROR: Cannot open output file %s", filename);

        // Convert word count to bytes
        numBytes = numOutputsWords * 4;
        numBytes = (numBytes > MAX_IO_SIZE) ? MAX_IO_SIZE : numBytes;

        // --- write each 32-bit word ---
        for (idx = 0; idx <= numBytes-4; idx += 4) begin
            // reconstruct little-endian int32 from 4 bytes
            value = {outputBytes[idx+3], outputBytes[idx+2],
                    outputBytes[idx+1], outputBytes[idx]};

            $fwrite(fd, "%0d\n", value);
        end

        $fclose(fd);
    endtask

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

        $dumpfile("waveform_prog.vcd");
        $dumpvars(5, dut);

        memEn = 0;
        memData = 0;
        memAddr = 0;
        sim_state = INIT;

        $readmemh("prog/program.txt", dut.mainMemory);
        readInputData("iofiles/input.txt");

        while ((sim_time < MAX_SIM_TIME + MAX_IO_SIZE) && is_timeout) begin
            @(posedge clock);

            case (sim_state)
                INIT: sim_state = SEND_DATA;

                SEND_DATA: begin
                    memData[7:0] = inputBytes[data_idx];
                    memAddr = DIN_ADDR + data_idx;
                    memEn = 1;
                    data_idx = data_idx + 1;
                    $display("t=%d Loaded %x", sim_time, memData[7:0]);

                    if (data_idx == inputSize || data_idx == MAX_IO_SIZE) begin
                        data_idx = 0;
                        sim_state = RESET;
                    end
                end

                RESET: begin
                    $display("t=%d Reset", sim_time);
                    reset = 1;
                    memData = 0;
                    memAddr = 0;
                    memEn = 0;
                    #20 reset = 0;
                    sim_state = RUN;
                end

                RUN: begin
                    if (dut.pc == 'h14) begin   // Program halt
                        outputBytes = dut.mainMemory[DOUT_ADDR+:MAX_IO_SIZE];
                        numOutputs = a0;
                        is_timeout = 0;

                        writeOutputData("iofiles/output.txt", numOutputs);
                        $display("Return code: %d\n", a0);
                    end
                end

                default: sim_state = INIT;
            endcase

            sim_time = sim_time + 1;
        end

        if (is_timeout) begin
            $display("Time Out\n");
        end
        
        $finish;
    end

endmodule