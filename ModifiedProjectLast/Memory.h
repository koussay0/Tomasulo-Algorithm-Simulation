#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

class Memory {
private:
    static constexpr int MEM_SIZE = 65536; // 2^16 = 64K words = 128 KB
    std::array<uint16_t, MEM_SIZE> data{};
public:
    Memory() { data.fill(0); }
    void loadData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Failed to open memory file");
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int address, value;
            if (!(iss >> address >> value)) continue;
            write(address, value);
        }
    }
    uint16_t read(uint16_t address) const {
        if (address >= MEM_SIZE)
            throw std::out_of_range("Memory read out of bounds");
        return data[address];
    }
    void write(uint16_t address, uint16_t value) {
        if (address >= MEM_SIZE)
            throw std::out_of_range("Memory write out of bounds");
        data[address] = value & 0xFFFF;
    }
};
