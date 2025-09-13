#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "user_func.h"

#define MAX_IO_SIZE 256 >> 2

int readData(std::string filename, std::vector<int> &data);
int writeData(std::string filename, const int *dout, int numOutputs);

int main() {
    
    std::vector<int> input_data;
    int status = readData("iofiles/input.txt", input_data);
    if (status != 0) return 1;

    int *din = input_data.data();
    int dout[MAX_IO_SIZE] = {0};

    int num_outputs = user_func(din, dout);
    num_outputs = (num_outputs > MAX_IO_SIZE) ? MAX_IO_SIZE : num_outputs;

    std::cout << "Function output: " << dout[0] << std::endl;

    std::vector<int> sim_output_data;
    status = readData("iofiles/output.txt", sim_output_data);
    if (status != 0) return 0;

    writeData("iofiles/sw_output.txt", dout, num_outputs);

    bool matched = true;
    for (int i = 0; i < num_outputs; i++) {
        if (dout[i] != sim_output_data[i]) {
            std::cout << "Mismatched output: sim " << sim_output_data[i] << ", sw " << dout[i] << std::endl;
            matched = false;
            break;
        }
    }

    if (matched) {
        std::cout << "PASS: Software and simulation outputs match\n";
        return 0;
    } else {
        std::cout << "FAIL: mismatched software and simulation outputs\n";
        return 1;
    }
}


int readData(std::string filename, std::vector<int> &data) {

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
            int value = std::stoi(token, &pos, 10);
            data.push_back(value);
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid input: " << token << "\n";
            continue;
        }
    }

    // // Debug print
    // for (auto b : data) {
    //     std::cout << b << " ";
    // }
    // std::cout << std::endl;

    return 0;
}

int writeData(std::string filename, const int *dout, int num_outputs) {

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    for (int i = 0; i < num_outputs; i++) {
        file << dout[i] << "\n";
    }

    return 0;
}