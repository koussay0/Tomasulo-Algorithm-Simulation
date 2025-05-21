//
// Created by ismai on 5/21/2025.
//

#ifndef C_INSTRUCTION_H
#define C_INSTRUCTION_H
#include <string>
#include <vector>
using namespace std;


class Instruction {
public:
    struct InstructionTiming {
        int issue = -1;
        int startExec = -1;
        int endExec = -1;
        int writeBack = -1;
    };
    string op;
    vector<string> args;
    InstructionTiming timing;
    bool isBranch = false;
    bool branchTaken = false;
    bool branchPredictedTaken = false;
};

#endif //C_INSTRUCTION_H
