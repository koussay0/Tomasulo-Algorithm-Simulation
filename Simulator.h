#pragma once
#include "Instruction.h"
#include "Memory.h"
#include "RegisterRenaming.h"
#include "ReservationStation.h"
#include "PipelineStage.h"
#include <vector>

class Simulator {
private:
    std::vector<Instruction> program;
    Memory memory;
    RegisterRenaming renamer;
    std::vector<ReservationStation> reservationStations;
    std::vector<PipelineStage> pipeline;
    int cycle;
    int instructionsCompleted;
    int branches;
    int mispredictions;

public:
    Simulator();
    void loadProgram(const std::string& filename);
    void loadData(const std::string& filename);
    void run();
    void printResults();
};