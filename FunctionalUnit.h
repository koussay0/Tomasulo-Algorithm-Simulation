#include "Memory.h"

class FunctionalUnit {
private:
    std::string opcode;
    int remCycles;
    int PC;
    int Label;
    int16_t i1, i2, offset;
    int16_t result;

public:
    Memory* memory; // This should be set externally

    FunctionalUnit(std::string Opcode, int Cycles, int pc, int label, int16_t I1, int16_t I2, int16_t Offset);

    int16_t Operation();
    int getRemCycles();
    void clear();
};
