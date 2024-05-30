#include <cstdio>
#include <iterator>
#include <ostream>
#include <stdio.h>
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

int algo_passed = 0;

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        int a = 0;
        int b = 1;
        int temp;
        for (int i = 2; i <= n; i++) {
            temp = a;
            a = b;
            b = temp + b;
        }
        return b;
    }
}

#define MAX 100
int sieve_of_eratosthenes(int n) {
    // Array to store prime status
    int prime[MAX + 1];
    int count = 0;

    // Initialize all entries as true (1).
    for (int i = 0; i <= n; i++) {
        prime[i] = 1;
    }

    for (int p = 2; p * p <= n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == 1) {
            // Update all multiples of p to not prime
            for (int i = p * p; i <= n; i += p) {
                prime[i] = 0;
            }
        }
    }

    for (int p = 2; p <= n; p++) {
        if (prime[p] == 1) {
            count = count + 1;
        }
    }
    return count;
}

void runSimulation(const std::string& filename) {
    #define MAX_SIM_TIME   100000
    vluint64_t sim_time =   0;
    vluint64_t posedge_cnt =   0;
    int test_no = 1;
    
    Vprocessor *dut = new Vprocessor;
    // Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    // dut->trace(m_trace,   5);
    
    // m_trace->open("waveform.vcd");

    

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
                dut->insMemData = hexVector[posedge_cnt];
                dut->insMemAddr = posedge_cnt;
                dut->insMemEn =   1;
                posedge_cnt++;
            }
        }

        //Resetting for one clock cycle 
        if (sim_time >=   3 + size && sim_time <=   4 + size) {
            dut->reset =   1;
            dut->insMemData =   0;
            dut->insMemAddr =   0;
            dut->insMemEn =   0;
        }

        dut->eval();
        //Simulation body

        //Verifying
        if (filename == "algorithms/fib.dat") {
            if (dut->a0 == fibonacci(10)) {
                std::cout << "Fibonacci sequence passed \n"<< std::endl;
                algo_passed++;
                break;
            }
        }
        if (filename == "algorithms/prime.dat") {
            if (dut->a0 == sieve_of_eratosthenes(50)) {
                std::cout << "Prime number generator passed \n"<< std::endl;
                algo_passed++;
                break;
            }
        }

        // m_trace->dump(sim_time);
        sim_time++;
        if (sim_time >= MAX_SIM_TIME  + size) {
            std::cout << "Prime number generator failed \n"<< std::endl;
        }
    }

    // m_trace->close();
    delete dut;
}

int main(int argc, char** argv, char** env) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("algorithms")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.size() >   4 && filename.substr(filename.size() -   4) == ".dat") {
                std::string fullPath = "algorithms/" + filename;
                std::cout << "Processing file: " << fullPath << std::endl;
                runSimulation(fullPath);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Could not open directory" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "\nTotal algorithms pass: " << algo_passed << std::endl;
    return EXIT_SUCCESS;
}