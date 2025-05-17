

#ifndef C_FUNCTIONALUNIT_H
#define C_FUNCTIONALUNIT_H

#include <string>
#include <cstdint>
using namespace std;

class FunctionalUnit {
public:
    string opcode;
    int PC;
    int Label;
    int16_t i1, i2, offset, result;

    FunctionalUnit(string opcode, int PC, int Label, int16_t i1, int16_t i2, int16_t offset, int16_t result);
    int16_t Operation();
};


#endif //C_FUNCTIONALUNIT_H
