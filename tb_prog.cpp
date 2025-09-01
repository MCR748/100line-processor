#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <string>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vprocessor.h"

// Change simulation time to number of cycles required
#define MAX_SIM_TIME 100000

#define DIN_ADDR 0x3E00
#define DOUT_ADDR 0x3F00
#define MAX_IO_SIZE 256    // Bytes

int readProgram(std::string filename, std::vector<unsigned int> &hexVector);
int readInputData(std::string filename, std::vector<uint8_t> &inputBytes);
int writeOutputData(std::string filename, const uint8_t *outputBytes);

enum State {
  INIT,
  SEND_INST,
  SEND_DATA,
  RESET,
  RUN
};

//To keep track of the current cycle
vluint64_t sim_time = 0;
//If need to count posedges
vluint64_t cycle_count = 0;

bool is_timeout = true;

State sim_state = INIT;

int main(int argc, char** argv, char** env) {
    //Replace $(Module-name) with the defined module name in chisel 
    Vprocessor *dut = new Vprocessor;

    //To generate trace
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);     //Show signals down to 5 levels deep
    m_trace->open("waveform_prog.vcd");

    //Reading the dat file
    std::vector<unsigned int> hexVector;
    int status = readProgram("prog/program.txt", hexVector);
    if (status != 0) return 1;

    int progSize = hexVector.size() * 2;      //Get twice the size of vector, as 1 cycle = 2 sim_time

    std::vector<uint8_t> inputBytes;
    status = readInputData("iofiles/input.txt", inputBytes);
    if (status != 0) return 1;

    int inpSize = inputBytes.size();

    //Simulation loop
    while (sim_time < MAX_SIM_TIME + progSize + inpSize) {

        dut->reset =   0;
        dut->clock ^=   1;

        switch (sim_state) {

        case INIT:
            // Wait 1 sim_time
            sim_state = SEND_INST;
            break;
        
        case SEND_INST:
            if (dut->clock == 1) {
                //printf("t=%lu Loaded %lu\n", sim_time, cycle_count);
                dut->memData = hexVector[cycle_count];
                dut->memAddr = cycle_count;
                dut->memEn =   1;
                cycle_count++;
                if (cycle_count == hexVector.size()) {
                    cycle_count = 0;
                    sim_state = SEND_DATA;
                }
            }
            break;

        case SEND_DATA:
            if (dut->clock == 1) {
                printf("t=%lu Loaded %lu\n", sim_time, cycle_count);
                dut->memData = inputBytes[cycle_count];
                dut->memAddr = DIN_ADDR + cycle_count;
                dut->memEn =   1;
                cycle_count++;
                if ((cycle_count == inputBytes.size() || cycle_count == MAX_IO_SIZE)) {
                    cycle_count = 0;
                    sim_state = RESET;
                }
            }
            break;

        case RESET:
            printf("t=%lu Reset\n", sim_time);
            dut->reset =   1;
            dut->memData =   0;
            dut->memAddr =   0;
            dut->memEn =   0;
            if (dut->clock == 1) sim_state = RUN;
            break;
        }

        //Add input signals here
        //Eval will set the signals in and make them reflect in output
        
        dut->eval();

        //Add any checks here for output
        //Simulation body
        if (sim_state == RUN && dut->processor__DOT__pc == 0x14) {  // Program halt

            const uint8_t *outputBytes = reinterpret_cast<const uint8_t*>(&dut->processor__DOT__mainMemory[DOUT_ADDR]);
            writeOutputData("iofiles/output.txt", outputBytes);

            int din = *reinterpret_cast<const int32_t*>(&dut->processor__DOT__mainMemory[DIN_ADDR]);
            int dout = *reinterpret_cast<const int32_t*>(&dut->processor__DOT__mainMemory[DOUT_ADDR]);
            std::cout << "t=" << sim_time << " Program input: " << din << " Program output: " << dout << std::endl;
            std::cout << "Return code: " << dut->a0 << std::endl;
            is_timeout = false;
            break;
        }

        m_trace->dump(sim_time);
        sim_time++;
    }

    if (is_timeout) {
        std::cout << "Time Out" << std::endl;
        std::cout << std::endl;
        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}

int readProgram(std::string filename, std::vector<unsigned int> &hexVector) {

    //Reading the dat file
    std::ifstream inputFile(filename); 
    std::string line;

    if (inputFile.is_open()) {

        while (std::getline(inputFile, line)) {
            try {
                unsigned int hexValue = std::stoul(line, nullptr,  16);
                hexVector.push_back(hexValue);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return -1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range error: " << e.what() << std::endl;
                return -1;
            }
        }

        inputFile.close();
        return 0;

    } else {
        std::cerr << "Unable to open file" << std::endl;
        return -1;
    }
}

int readInputData(std::string filename, std::vector<uint8_t> &inputBytes) {

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    int32_t value;
    std::string token;

    while (file >> token) {
        try {
            size_t pos = 0;
            long long temp = std::stoll(token, &pos, 10);

            // Ensure whole token was parsed and fits in int32_t
            if (pos != token.size() || temp < INT32_MIN || temp > INT32_MAX) {
                std::cerr << "Invalid integer: " << token << "\n";
                continue; // skip invalid input
            }

            value = static_cast<int32_t>(temp);

            // Break into bytes (little-endian)
            for (int i = 0; i < 4; ++i) {
                inputBytes.push_back(static_cast<uint8_t>(value & 0xFF));
                value = value >> 8;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid input: " << token << "\n";
            continue;
        }
    }

    if (inputBytes.size() > MAX_IO_SIZE) {
        std::cout << "WARNING: Input size " << inputBytes.size() << " is greater than " << MAX_IO_SIZE << std::endl;
    }

    // Debug print
    for (auto b : inputBytes) {
        printf("%0x ", b);
    }
    printf("\n");

    return 0;
}

int writeOutputData(std::string filename, const uint8_t *outputBytes) {

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    for (int i = 0; i < MAX_IO_SIZE; i+=4) {
        int32_t value = *reinterpret_cast<const int32_t*>(outputBytes + i);
        file << value << "\n";
    }

    return 0;
}