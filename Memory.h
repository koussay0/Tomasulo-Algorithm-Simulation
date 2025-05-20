#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <sstream>

class Memory {
private:
    static constexpr int MEM_SIZE = 65536; // 2^16 = 64K words = 128 KB
    std::array<uint16_t, MEM_SIZE> data{}; // 16-bit word-addressable memory

public:
    Memory() {
        data.fill(0);
    }

    // Load data from file with format: address value
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

    // Read a 16-bit word from memory
    uint16_t read(uint16_t address) const {
        if (address >= MEM_SIZE)
            throw std::out_of_range("Memory read out of bounds");
        return data[address];
    }

    // Write a 16-bit word to memory
    void write(uint16_t address, uint16_t value) {
        if (address >= MEM_SIZE)
            throw std::out_of_range("Memory write out of bounds");
        data[address] = value & 0xFFFF; // ensure 16-bit
    }

    // For debugging: print memory range
    void dump(uint16_t start, uint16_t end) const {
        for (uint16_t addr = start; addr <= end; ++addr) {
            printf("Mem[0x%04X] = 0x%04X\n", addr, data[addr]);
        }
    }
};
