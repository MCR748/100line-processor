#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vprocessor.h"

// Change simulation time to number of cycles required
#define MAX_SIM_TIME 1000
//To keep track of the current cycle
vluint64_t sim_time = 0;
//If need to count posedges
vluint64_t posedge_cnt = 0;
bool is_timeout = true;



int main(int argc, char** argv, char** env) {
    //Replace $(Module-name) with the defined module name in chisel 
    Vprocessor *dut = new Vprocessor;

    //To generate trace
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);     //Show signals down to 5 levels deep
    m_trace->open("waveform_prog.vcd");

    //Reading the dat file
    std::ifstream inputFile("prog/program.txt"); 
    std::vector<unsigned int> hexVector;
    std::string line;

    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            try {
                unsigned int hexValue = std::stoul(line, nullptr,  16);
                hexVector.push_back(hexValue);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range error: " << e.what() << std::endl;
            }
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    int size = hexVector.size()*2;      //Get twice the size of vector, as 1 cycle = 2 sim_time

    //Simulation loop
    while (sim_time < MAX_SIM_TIME + size) {
        dut->reset =   0;

        dut->clock ^=   1;

        //Initailizing the vector values
        if (dut->clock ==   1) {
            if (sim_time >=   1 && sim_time <= size +   1) {
                dut->memData = hexVector[posedge_cnt];
                dut->memAddr = posedge_cnt;
                dut->memEn =   1;
                posedge_cnt++;
            }
        }

        //Resetting for one clock cycle 
        if (sim_time >=   3 + size && sim_time <=   4 + size) {
            dut->reset =   1;
            dut->memData =   0;
            dut->memAddr =   0;
            dut->memEn =   0;
        }
            
        //Add input signals here
        //Eval will set the signals in and make them reflect in output
        
        dut->eval();

        //Add any checks here for output
        //Simulation body

        m_trace->dump(sim_time);
        sim_time++;
    }
    if(is_timeout) {
            std::cout << "Time Out" << std::endl;
            std::cout << std::endl;
            m_trace->dump(sim_time);
            sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
