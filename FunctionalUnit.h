

#ifndef C_FUNCTIONALUNIT_H
#define C_FUNCTIONALUNIT_H

#include <string>
#include <cstdint>
#include "Memory.h"
using namespace std;

class FunctionalUnit {
public:
    string opcode;
    int PC;
    int Label;
    int remCycles;
    int16_t i1, i2, offset, result;
    Memory memory;

    FunctionalUnit(string Opcode, int Cycles, int pc, int label, int16_t I1, int16_t I2, int16_t Offset);
    int16_t Operation();
    int getRemCycles();
    void clear();
};


#endif //C_FUNCTIONALUNIT_H
