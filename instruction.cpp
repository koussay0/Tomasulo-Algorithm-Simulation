// TomasuloSimulator.cpp
#include "instruction.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

TomasuloSimulator::TomasuloSimulator(const std::string& filename) {
    instructions = readInstructions(filename);
}

std::vector<Instruction> TomasuloSimulator::readInstructions(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Instruction> instructions;

    if (!file.is_open()) {
        std::cerr << "Cannot open instruction file at path: " << filename << "\n";
        return instructions;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        Instruction instr;
        ss >> instr.op;

        std::string arg;
        while (ss >> arg) {
            if (!arg.empty() && arg.back() == ',') arg.pop_back();
            instr.args.push_back(arg);
        }

        if (instr.op == "BEQ" || instr.op == "CALL" || instr.op == "RET") {
            instr.isBranch = true;
        }

        instructions.push_back(instr);
    }

    return instructions;
}

int TomasuloSimulator::getExecutionCycles(const std::string& op) const {
    if (op == "LOAD" || op == "STORE") return 6;
    if (op == "ADD" || op == "SUB") return 4;
    if (op == "MUL") return 10;
    if (op == "NOR") return 2;
    if (op == "BEQ" || op == "CALL" || op == "RET") return 1;
    return 1;
}

void TomasuloSimulator::simulate() {
    int cycle = 0;

    for (auto& instr : instructions) {
        instr.timing.issue = cycle++;
        instr.timing.startExec = cycle;
        instr.timing.endExec = cycle + getExecutionCycles(instr.op) - 1;
        instr.timing.writeBack = instr.timing.endExec + 1;
        cycle = instr.timing.writeBack + 1;

        if (instr.isBranch) {
            branchCount++;
            instr.branchPredictedTaken = false;  // dummy prediction
            instr.branchTaken = true;            // dummy actual behavior
            if (instr.branchPredictedTaken != instr.branchTaken) {
                mispredictedBranches++;
            }
        }

        completedInstructions++;
    }

    totalCycles = cycle;
}

void TomasuloSimulator::printResults() const {
    std::cout << std::left << std::setw(10) << "Index"
              << std::setw(10) << "Op"
              << std::setw(10) << "Issue"
              << std::setw(10) << "Start"
              << std::setw(10) << "End"
              << std::setw(10) << "Write" << "\n";

    for (size_t i = 0; i < instructions.size(); ++i) {
        const auto& instr = instructions[i];
        std::cout << std::setw(10) << i
                  << std::setw(10) << instr.op
                  << std::setw(10) << instr.timing.issue
                  << std::setw(10) << instr.timing.startExec
                  << std::setw(10) << instr.timing.endExec
                  << std::setw(10) << instr.timing.writeBack << "\n";
    }

    std::cout << "\nTotal Cycles: " << totalCycles << "\n";
    std::cout << "IPC: " << static_cast<float>(completedInstructions) / totalCycles << "\n";
    std::cout << "Branch Misprediction Rate: "
              << (branchCount > 0 ? (100.0f * mispredictedBranches / branchCount) : 0.0f)
              << "%\n";
}
