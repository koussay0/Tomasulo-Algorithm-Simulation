#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "FunctionalUnit.h"



//We're missing an access to the memory

FunctionalUnit::FunctionalUnit(std::string Opcode, int Cycles, int pc, int label, int16_t I1, int16_t I2, int16_t Offset) {
    opcode = Opcode;
    remCycles = Cycles;
    PC = pc;
    Label = label;
    i1 = I1;
    i2 = I2;
    offset = Offset;
    result = 0;
    //result = Result;
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
        result = (i1 * i2) & 0xFFFF;
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
        result = offset + i2;
    }
    else if (opcode == "STORE") {
        result = offset + i2;
    }
    else if (opcode == "BEQ") {
        if (i1 == i2) {
            PC = PC+1+offset;
            result = PC;
        } else {
            PC = PC+1;
            result = PC;
        }
    }
    else {
        result = 0;
    }
    return result;
}

int FunctionalUnit ::getRemCycles() {
    return remCycles;
}

void FunctionalUnit ::clear() {
    opcode = "";
    i1 = 0;
    i2 = 0;
    offset = 0;
    result = 0;
    remCycles = 0;
}


