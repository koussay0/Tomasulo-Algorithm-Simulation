// TomasuloSimulator.hpp
#ifndef TOMASULO_SIMULATOR_HPP
#define TOMASULO_SIMULATOR_HPP

#include <string>
#include <vector>
#include "ReservationStation.h"
#include "CommonDataBus.h"
#include "FunctionalUnit.h"

struct InstructionTiming {
    int issue = -1;
    int startExec = -1;
    int endExec = -1;
    int writeBack = -1;
};

struct Instruction {
    std::string op;
    std::vector<std::string> args;
    InstructionTiming timing;
    bool isBranch = false;
    bool branchTaken = false;
    bool branchPredictedTaken = false;
};

class TomasuloSimulator {
public:
    int PC;
    int cycle = 0;
    vector<int16_t> registerFile = vector<int16_t>(8, 0);
    vector<ReservationStation*> registerStatus;
    vector<ReservationStation> reservationStations;
    vector <FunctionalUnit> fuList;
    CommonDataBus commonDataBus;
    TomasuloSimulator(const std::string& filename);
    void simulate();
    void printResults() const;
    void Issue();
    void Execute();


    std::vector<Instruction> instructions;
    int totalCycles = 0;
    int completedInstructions = 0;
    int branchCount = 0;
    int mispredictedBranches = 0;

    std::vector<Instruction> readInstructions(const std::string& filename);
    int getExecutionCycles(const std::string& op) const;
};

#endif // TOMASULO_SIMULATOR_HPP
