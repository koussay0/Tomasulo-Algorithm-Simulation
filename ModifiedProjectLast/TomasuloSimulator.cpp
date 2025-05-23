#include "TomasuloSimulator.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

TomasuloSimulator::TomasuloSimulator(const std::string& instrFile, const std::string& dataFile) {
    instructions = readInstructions(instrFile);
    memory.loadData(dataFile);
    registerFile = RegisterFile();
    PC = 0;
    // Reservation stations: 2 LOAD, 2 STORE, 2 BEQ, 1 CALL/RET, 4 ADD/SUB, 2 NOR, 1 MUL
    for (int i = 0; i < 2; ++i) reservationStations.push_back({"LOAD" + std::to_string(i), "", 0});
    for (int i = 0; i < 2; ++i) reservationStations.push_back({"STORE" + std::to_string(i), "", 0});
    for (int i = 0; i < 2; ++i) reservationStations.push_back({"BEQ" + std::to_string(i), "", 0});
    reservationStations.push_back({"CALLRET0", "", 0});
    for (int i = 0; i < 4; ++i) reservationStations.push_back({"ADD" + std::to_string(i), "", 0});
    for (int i = 0; i < 2; ++i) reservationStations.push_back({"NOR" + std::to_string(i), "", 0});
    reservationStations.push_back({"MUL0", "", 0});
}

std::vector<Instruction> TomasuloSimulator::readInstructions(const std::string& filename) {
    std::vector<Instruction> instrs;
    std::ifstream file(filename);
    std::string line;
    int addr = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string op;
        std::vector<std::string> args;
        iss >> op;
        std::string arg;
        while (iss >> arg) args.push_back(arg);
        instrs.push_back(Instruction(op, args, addr++));
    }
    return instrs;
}

int TomasuloSimulator::getExecutionCycles(const std::string& op) const {
    if (op == "LOAD") return 6;
    if (op == "STORE") return 6;
    if (op == "BEQ") return 1;
    if (op == "CALL" || op == "RET") return 1;
    if (op == "ADD" || op == "SUB") return 4;
    if (op == "NOR") return 2;
    if (op == "MUL") return 10;
    return 1;
}

void TomasuloSimulator::simulate() {
    cycle = 0;
    completedInstructions = 0;
    while (completedInstructions < (int)instructions.size()) {
        Issue();
        Execute();
        Write();
        for (auto& instr : instructions) {
            if (instr.timing.writeBack == cycle) {
                completedInstructions++;
            }
        }
        cycle++;
    }
    totalCycles = cycle;
}

void TomasuloSimulator::Issue() {
    if (PC >= (int)instructions.size()) return;
    Instruction& instr = instructions[PC];
    ReservationStation* rs = nullptr;
    for (auto& station : reservationStations) {
        if (station.busy == 0) {
            rs = &station;
            break;
        }
    }
    if (!rs) return;
    instr.timing.issue = cycle;
    rs->busy = 1;
    rs->op = instr.op;
    rs->address = instr.address;
    rs->execCycles = getExecutionCycles(instr.op);
    rs->ready = false;
    instr.timing.startExec = -1;
    instr.timing.endExec = -1;
    instr.timing.writeBack = -1;
    ++PC;
}

void TomasuloSimulator::Execute() {
    for (auto& rs : reservationStations) {
        if (rs.busy && rs.execCycles > 0) {
            Instruction* instr = nullptr;
            for (auto& i : instructions) {
                if (i.address == rs.address) {
                    instr = &i;
                    break;
                }
            }
            if (!rs.ready) {
                rs.ready = true;
                if (instr && instr->timing.startExec == -1)
                    instr->timing.startExec = cycle;
            }
            rs.execCycles--;
            if (rs.execCycles == 0) {
                rs.writeResultCycle = cycle + 1;
                if (instr) instr->timing.endExec = cycle;
            }
        }
    }
}

void TomasuloSimulator::Write() {
    for (auto& rs : reservationStations) {
        if (rs.busy && rs.execCycles == 0 && rs.writeResultCycle == cycle) {
            for (auto& instr : instructions) {
                if (instr.address == rs.address && instr.timing.writeBack == -1) {
                    instr.timing.writeBack = cycle;
                    break;
                }
            }
            rs.busy = 0;
            rs.op = "";
        }
    }
}

void TomasuloSimulator::printResults() const {
    std::cout << "Instr | Issue | Start | End | WB\n";
    std::cout << "-------------------------------\n";
    for (const auto& instr : instructions) {
        std::cout << std::setw(5) << instr.op << " | "
                  << std::setw(5) << instr.timing.issue << " | "
                  << std::setw(5) << instr.timing.startExec << " | "
                  << std::setw(3) << instr.timing.endExec << " | "
                  << std::setw(3) << instr.timing.writeBack << "\n";
    }
    std::cout << "Total Cycles: " << totalCycles << std::endl;
    std::cout << "IPC: " << (double)instructions.size() / (totalCycles ? totalCycles : 1) << std::endl;
    std::cout << "Branch Misprediction Rate: " << (branchCount ? (100.0 * mispredictedBranches / branchCount) : 0) << "%\n";
}
