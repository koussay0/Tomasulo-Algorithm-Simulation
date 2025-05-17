#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "FunctionalUnit.h"



//We're missing an access to the memory

FunctionalUnit::FunctionalUnit(std::string opcode, int PC, int Label, int16_t i1, int16_t i2, int16_t offset, int16_t result) {
    opcode = opcode;
    PC = PC;
    Label = Label;
    i1 = i1;
    i2 = i2;
    offset = offset;
    result = result;
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


