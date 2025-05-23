#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Instruction.h"
#include "Memory.h"
using namespace std;

struct RegisterFile {
    vector<int16_t> regs = std::vector<int16_t>(8, 0);
    int16_t read(int idx) const { return idx == 0 ? 0 : regs[idx]; }
    void write(int idx, int16_t val) { if (idx != 0) regs[idx] = val; }
};

struct ReservationStation {
    string name;
    string op;
    int busy = 0;
    int Vj = 0, Vk = 0;
    int Qj = -1, Qk = -1;
    int dest = -1;
    int address = -1;
    int execCycles = 0;
    bool ready = false;
    int writeResultCycle = -1;
    int offset = 0;
    int label = 0;
};

struct FunctionalUnit {
    string type;
    int i1 = 0, i2 = 0, offset = 0;
    FunctionalUnit(const std::string& t, int a=0, int b=0, int c=0) : type(t), i1(a), i2(b), offset(c) {}
    int16_t Operation(const std::string& op) {
        if (op == "ADD") return i1 + i2;
        if (op == "SUB") return i1 - i2;
        if (op == "MUL") return i1 * i2;
        if (op == "NOR") return ~(i1 | i2);
        if (op == "LOAD" || op == "STORE") return i1 + offset;
        return 0;
    }
};

class TomasuloSimulator {
public:
    TomasuloSimulator(const std::string& instrFile, const std::string& dataFile);
    void simulate();
    void printResults() const;
private:
    int PC = 0;
    int cycle = 0;
    int totalCycles = 0;
    int completedInstructions = 0;
    int branchCount = 0;
    int mispredictedBranches = 0;
    vector<Instruction> instructions;
    RegisterFile registerFile;
    vector<ReservationStation> reservationStations;
    vector<FunctionalUnit> functionalUnits;
    Memory memory;
    void Issue();
    void Execute();
    void Write();
    int getExecutionCycles(const std::string& op) const;
    vector<Instruction> readInstructions(const std::string& filename);
};
