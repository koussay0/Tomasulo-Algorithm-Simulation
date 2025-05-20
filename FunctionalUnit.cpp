#include <iostream>
#include <string>
#include <cstdint>
#include "FunctionalUnit.h"
#include "Memory.h" // Make sure this matches your filename

FunctionalUnit::FunctionalUnit(std::string Opcode, int Cycles, int pc, int label, int16_t I1, int16_t I2, int16_t Offset) {
    opcode = Opcode;
    remCycles = Cycles;
    PC = pc;
    Label = label;
    i1 = I1;
    i2 = I2;
    offset = Offset;
    result = 0;
    memory = nullptr; // pointer to memory must be set externally
}

int16_t FunctionalUnit::Operation() {
    if (opcode == "ADD") {
        result = i1 + i2;
    }
    else if (opcode == "SUB") {
        result = i1 - i2;
    }
    else if (opcode == "NOR") {
        result = ~(i1 | i2);
    }
    else if (opcode == "MUL") {
        result = (i1 * i2) & 0xFFFF; // 16-bit result
    }
    else if (opcode == "CALL") {
        result = PC + 1;
        PC = Label;
    }
    else if (opcode == "RET") {
        PC = i1;
        result = PC;
    }
    else if (opcode == "LOAD") {
        int16_t effectiveAddr = i2 + offset;
        if (memory != nullptr) {
            result = memory->read(effectiveAddr);
        } else {
            std::cerr << "Memory not set for LOAD operation\n";
            result = 0;
        }
    }
    else if (opcode == "STORE") {
        int16_t effectiveAddr = i2 + offset;
        if (memory != nullptr) {
            memory->write(effectiveAddr, i1);
        } else {
            std::cerr << "Memory not set for STORE operation\n";
        }
        result = effectiveAddr;
    }
    else if (opcode == "BEQ") {
        if (i1 == i2) {
            PC = PC + 1 + offset;
        } else {
            PC = PC + 1;
        }
        result = PC;
    }
    else {
        result = 0;
    }
    return result;
}

int FunctionalUnit::getRemCycles() {
    return remCycles;
}

void FunctionalUnit::clear() {
    opcode = "";
    i1 = 0;
    i2 = 0;
    offset = 0;
    result = 0;
    remCycles = 0;
}
