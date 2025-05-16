#include "Simulator.h"
#include <fstream>
#include <iostream>
#include <sstream>

Simulator::Simulator()
    : cycle(0), instructionsCompleted(0), branches(0), mispredictions(0) {}

void Simulator::loadProgram(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        program.push_back(Instruction::fromLine(line));
    }
}

void Simulator::loadData(const std::string& filename) {
    memory.loadData(filename);
}

void Simulator::run() {
    // Simple loop placeholder: Add issue, execute, writeback logic later
    for (const auto& instr : program) {
        cycle++;
        std::cout << "Cycle " << cycle << ": Issued instruction: ";
        instr.print();
        instructionsCompleted++;
        if (instr.op == "BEQ") branches++;
    }
}

void Simulator::printResults() {
    std::cout << "\nSimulation completed in " << cycle << " cycles.\n";
    std::cout << "Instructions completed: " << instructionsCompleted << "\n";
    std::cout << "IPC: " << static_cast<float>(instructionsCompleted) / cycle << "\n";
    std::cout << "Branches: " << branches << ", Mispredictions: " << mispredictions << "\n";
    if (branches > 0)
        std::cout << "Misprediction rate: " << (100.0 * mispredictions / branches) << "%\n";
}