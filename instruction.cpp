// TomasuloSimulator.cpp
#include "instruction.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

TomasuloSimulator::TomasuloSimulator(const std::string& filename) {
    instructions = readInstructions(filename);
    PC = 0;
    registerStatus.resize(8, nullptr);
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


void TomasuloSimulator :: issue(){
    if (PC >= instructions.size()) return; // No more instructions

    Instruction& instr = instructions[PC]; //read the instruction

    // 1. Check for structural hazards - find an available reservation station
    ReservationStation* rs = nullptr;
    for (auto& station :  reservationStations) {
        if ((station.Name.find(instr.op) != string::npos) && !station.isBusy()) {
            rs = &station;
            break;
        }
    }

    // If no reservation station is available, cannot issue (structural hazard)
    if (rs == nullptr) {
        return;
    }

    instr.timing.issue = cycle;
    rs->Busy = true;
    rs->Op = instr.op;
    rs->instructionIndex = PC;
    rs->Qj = rs->Qk = nullptr;

    if (instr.op == "LOAD") {
        std::string baseReg = instr.args[1];
        baseReg = baseReg.substr(baseReg.find('(') + 1);
        baseReg = baseReg.substr(0, baseReg.find(')'));
        std::string destReg = instr.args[0];

        if (registerStatus[stoi(baseReg)]) {
            rs->Qj = registerStatus[stoi(baseReg)]->Name;
        } else {
            rs->Vj = registerFile[stoi(baseReg)];
        }

        registerStatus[stoi(destReg)] = rs;

    } else if (instr.op == "STORE") {
        std::string srcReg = instr.args[0];
        std::string baseReg = instr.args[1];
        baseReg = baseReg.substr(baseReg.find('(') + 1);
        baseReg = baseReg.substr(0, baseReg.find(')'));

        if (registerStatus[stoi(srcReg)]) {
            rs->Qj = registerStatus[stoi(srcReg)]->Name;
        } else {
            rs->Vj = registerFile[stoi(srcReg)];
        }

        if (registerStatus[stoi(baseReg)]) {
            rs->Qk = registerStatus[stoi(baseReg)]->Name;
        } else {
            rs->Vk = registerFile[stoi(baseReg)];
        }

    } else if (instr.op == "BEQ") {
        std::string rs1 = instr.args[0];
        std::string rs2 = instr.args[1];

        if (registerStatus[stoi(rs1)]) {
            rs->Qj = registerStatus[stoi(rs1)]->Name;
        } else {
            rs->Vj = registerFile[stoi(rs1)];
        }

        if (registerStatus[stoi(rs2)]) {
            rs->Qk = registerStatus[stoi(rs2)]->Name;
        } else {
            rs->Vk = registerFile[stoi(rs2)];
        }

    } else if (instr.op == "CALL") {
        // For CALL: no register dependencies, just mark RS busy with CALL op
        rs->Busy = true;
        rs->Op = "CALL";
        rs->Qj = nullptr;
        rs->Qk = nullptr;
        rs->Vj = 0;
        rs->Vk = 0;
        // You may want to track the return address register here if applicable
        // But your code comments say no dependencies assumed for now
    }
    else if (instr.op == "RET") {
        // For RET: assume it reads return address from a register (args[0])
        std::string retReg = instr.args[0];

        if (registerStatus[stoi(retReg)]) {
            rs->Qj = registerStatus[stoi(retReg)]->Name;
        } else {
            rs->Vj = registerFile[stoi(retReg)];
        }

        rs->Qk = nullptr;
        rs->Vk = 0;
        rs->Busy = true;
        rs->Op = "RET";
    }

   else {
        // ALU operations: ADD, SUB, MUL, NOR, etc.
        std::string destReg = instr.args[0];
        std::string src1 = instr.args[1];
        std::string src2 = instr.args[2];

        if (registerStatus[stoi(src1)]) {
            rs->Qj = registerStatus[stoi(src1)]->Name;
        } else {
            rs->Vj = registerFile[stoi(src1)];
        }

        if (registerStatus[stoi(src2)]) {
            rs->Qk = registerStatus[stoi(src2)]->Name;
        } else {
            rs->Vk = registerFile[stoi(src2)];
        }

        registerStatus[stoi(destReg)] = rs;
    }

    PC++; // Advance program counter after successful issue
}


