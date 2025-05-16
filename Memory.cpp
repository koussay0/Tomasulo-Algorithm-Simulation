#include "Memory.h"
#include <fstream>
#include <sstream>

void Memory::loadData(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        int address, value;
        ss >> address >> value;
        data[address] = value;
    }
}

int Memory::read(int address) {
    return data[address];
}

void Memory::write(int address, int value) {
    data[address] = value;
}