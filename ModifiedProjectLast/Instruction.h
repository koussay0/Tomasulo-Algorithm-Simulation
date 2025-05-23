#pragma once
#include <string>
#include <vector>

struct PipelineStage {
    int issue = -1;
    int startExec = -1;
    int endExec = -1;
    int writeBack = -1;
};

struct Instruction {
    std::string op;
    std::vector<std::string> args;
    int address = 0;
    PipelineStage timing;
    bool isBranch = false;
    bool branchTaken = false;
    bool branchPredictedTaken = false;
    Instruction(const std::string& op_ = "", const std::vector<std::string>& args_ = {}, int addr = 0)
        : op(op_), args(args_), address(addr), timing(), isBranch(false), branchTaken(false), branchPredictedTaken(false) {}
};
