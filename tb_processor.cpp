#include <cstdio>
#include <iterator>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <dirent.h> // For directory listing
#include <string>
#include "Vprocessor.h"

// Open output files for passed and failed tests
std::ofstream passedTestsFile("results/passed_tests.txt");
std::ofstream failedTestsFile("results/failed_tests.txt");

int tests_passed = 0;

void runSimulation(const std::string& filename) {
    #define MAX_SIM_TIME   1000
    vluint64_t sim_time =   0;
    vluint64_t posedge_cnt =   0;
    int test_no = 1;
    

    Vprocessor *dut = new Vprocessor;
    //Verilated::traceEverOn(true);
    //VerilatedVcdC *m_trace = new VerilatedVcdC;
    //dut->trace(m_trace,   5);
    
    //m_trace->open("waveform.vcd");

    std::ifstream inputFile(filename);
    std::vector<unsigned int> hexVector;
    std::string line;

    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            try {
                unsigned int hexValue = std::stoul(line, nullptr,   16);
                hexVector.push_back(hexValue);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range error: " << e.what() << std::endl;
            }
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
        return;
    }

    int size = hexVector.size()*2;      //Get twice the size of vector, as 1 cycle = 2 sim_time
    while (sim_time < MAX_SIM_TIME + size) {
        dut->reset =   0;

        dut->clock ^=   1;

        //Initailizing the vector values
        if (dut->clock ==   1) {
            if (sim_time >=   1 && sim_time <= size +   1) {
                dut->insMemDataIn = hexVector[posedge_cnt];
                dut->insMemAddr = posedge_cnt;
                dut->insMemEn =   1;
                posedge_cnt++;
            }
        }

        //Resetting for one clock cycle 
        if (sim_time >=   3 + size && sim_time <=   4 + size) {
            dut->reset =   1;
            dut->insMemDataIn =   0;
            dut->insMemAddr =   0;
            dut->insMemEn =   0;
        }

        dut->eval();
        //Simulation body

        //To get the number of the last test passed.
        if (dut-> clock == 1) {
            if (dut-> gp >= 2 & dut-> gp != 1) {
                test_no = dut->gp;
            }
        }
        //Verifying
        if (dut->a7 ==   93) {
            if (dut->gp == 1) {
                std::cout << "Test for file " << filename << " passed" << std::endl;
                std::cout << "Last test passed: " << test_no << std::endl;
                std::cout << std::endl;
                passedTestsFile << filename << std::endl; // Write to passed tests file
                passedTestsFile << "Last test passed : " << test_no << std::endl;
                tests_passed ++;
                // m_trace->dump(sim_time);
                sim_time++;
                break;
            } else {
                std::cout << "Test for file " << filename << " failed" << std::endl;
                failedTestsFile << filename << std::endl; // Write to failed tests file
                test_no = dut->gp >> 1;
                failedTestsFile << "Test failed at: " << test_no << std::endl;
                std::cout << "Test failed at: " << test_no << std::endl;
                std::cout << std::endl;
                // m_trace->dump(sim_time);
                sim_time++;
                break;

            }
        }

        //m_trace->dump(sim_time);
        sim_time++;
    }

    //m_trace->close();
    delete dut;
}

int main(int argc, char** argv, char** env) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("tests")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.size() >   4 && filename.substr(filename.size() -   4) == ".dat") {
                std::string fullPath = "tests/" + filename;
                std::cout << "Processing file: " << fullPath << std::endl;
                runSimulation(fullPath);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Could not open directory" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Total tests passed: " << tests_passed << std::endl;
    return EXIT_SUCCESS;
}
