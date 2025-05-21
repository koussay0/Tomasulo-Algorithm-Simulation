// TomasuloSimulator.cpp
#include "TomasuloSimulator.h"
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
    cycle = 0;
    completedInstructions = 0;
    branchCount = 0;
    mispredictedBranches = 0;

    while (completedInstructions < (int)instructions.size()) {
        Issue();
        Execute();
        Write();

        // Handle branch instructions
        for (auto& instr : instructions) {
            if (instr.isBranch && instr.timing.writeBack == cycle) {
                branchCount++;
                instr.branchPredictedTaken = false;  // Simple static prediction: predict not taken
                instr.branchTaken = true;            // Actual outcome determined in Write stage
                if (instr.branchPredictedTaken != instr.branchTaken) {
                    mispredictedBranches++;
                }
            }
        }

        // Update completion status
        for (auto& instr : instructions) {
            if (instr.timing.writeBack == cycle) {
                completedInstructions++;
            }
        }

        cycle++;
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


void TomasuloSimulator::Issue() {
    if (PC >= instructions.size()) return; // No more instructions

    Instruction& instr = instructions[PC]; //read the instruction

    // 1. Check for structural hazards - find an available reservation station
    ReservationStation* rs = nullptr;
    for (auto& station : reservationStations) {
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
    rs->destinationReg = -1;  // Initialize this
    rs->Qj = nullptr;  // Initialize these separately
    rs->Qk = nullptr;

    if (instr.op == "LOAD") {
        std::string baseReg = instr.args[1];
        baseReg = baseReg.substr(baseReg.find('(') + 1);
        baseReg = baseReg.substr(0, baseReg.find(')'));
        std::string destReg = instr.args[0];

        // Only set Qj if there's a dependency
        if (registerStatus[stoi(baseReg)]) {
            rs->Qj = registerStatus[stoi(baseReg)]->Name;
        } else {
            rs->Vj = registerFile[stoi(baseReg)];
        }

        rs->destinationReg = stoi(destReg);
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

        rs->destinationReg = -1;  // Store has no destination register

    } else if (instr.op == "BEQ") {
        std::string rs1 = instr.args[0];
        std::string rs2 = instr.args[1];
        std::string target = instr.args[2];  // Branch target

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

        rs->A = stoi(target);  // Store branch target
        rs->destinationReg = -1;  // Branch has no destination register

    } else if (instr.op == "CALL") {
        // For CALL: save return address
        rs->Busy = true;
        rs->Op = "CALL";
        rs->Qj = nullptr;
        rs->Qk = nullptr;
        rs->Vj = 0;
        rs->Vk = 0;
        rs->A = PC + 1;  // Save return address
        rs->destinationReg = 7;  // Assuming R7 is return address register
        registerStatus[7] = rs;  // Update status for return address register

    } else if (instr.op == "RET") {
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
        rs->destinationReg = -1;  // RET has no destination register

    } else {
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

        rs->destinationReg = stoi(destReg);
        registerStatus[stoi(destReg)] = rs;
    }

    rs->Issue_status = true;
    PC++; // Advance program counter after successful issue
}

void TomasuloSimulator::Execute() {
    // First handle CDB broadcasts to update waiting reservation stations
    if (commonDataBus.busy) {
        int cdbReg = commonDataBus.destinationReg;
        int16_t cdbValue = commonDataBus.value;

        for (auto& rs : reservationStations) {
            if (!rs.Busy || rs.Execute_status) continue;

            // Find the reservation station that produced this result
            for (auto& producerRS : reservationStations) {
                if (producerRS.Busy && producerRS.destinationReg == cdbReg) {
                    // If this RS is waiting on the producer's result
                    if (rs.Qj == producerRS.Name) {
                        rs.Vj = cdbValue;
                        rs.Qj.clear();
                    }
                    if (rs.Qk == producerRS.Name) {
                        rs.Vk = cdbValue;
                        rs.Qk.clear();
                    }
                    break;
                }
            }
        }
    }

    // Process each reservation station
    for (auto& rs : reservationStations) {
        if (!rs.Busy || !rs.Issue_status || rs.Execute_status)
            continue;

        // Check if operands are ready
        bool operandsReady = rs.Qj.empty() && rs.Qk.empty();
        if (!operandsReady) continue;

        // Start execution if not already started
        if (rs.functionalUnit == nullptr) {
            // Find a matching free functional unit
            for (auto& fu : fuList) {
                if (!fu.isBusy() && fu.opcode == rs.Op) {
                    // Configure the functional unit
                    fu.opcode = rs.Op;
                    fu.i1 = rs.Vj;
                    fu.i2 = rs.Vk;
                    fu.offset = rs.A;
                    fu.PC = rs.instructionIndex;  // Use instruction index instead of PC
                    fu.remCycles = getExecutionCycles(rs.Op);
                    fu.setBusy(true);

                    rs.functionalUnit = &fu;
                    instructions[rs.instructionIndex].timing.startExec = cycle;
                    break;
                }
            }
        }

        // Continue execution if functional unit is assigned
        if (rs.functionalUnit != nullptr) {
            if (rs.functionalUnit->remCycles > 0) {
                rs.functionalUnit->remCycles--;
            }

            // Check if execution is complete
            if (rs.functionalUnit->remCycles == 0) {
                instructions[rs.instructionIndex].timing.endExec = cycle;
                rs.result = rs.functionalUnit->Operation();  // Store result
                rs.Execute_status = true;

                // Free the functional unit
                rs.functionalUnit->clear();
                rs.functionalUnit = nullptr;
            }
        }
    }
}


void TomasuloSimulator::Write() {
    for (auto& rs : reservationStations) {
        if (!rs.Busy || !rs.Execute_status || rs.Write_status)
            continue;

        // Update instruction timing
        instructions[rs.instructionIndex].timing.writeBack = cycle;

        // Broadcast result to CDB (without sourceName since your implementation doesn't have it)
        commonDataBus.broadcast(rs.result, rs.destinationReg);

        // Update register file if this instruction has a destination register
        if (rs.destinationReg >= 0 && rs.destinationReg < (int)registerFile.size()) {
            registerFile[rs.destinationReg] = rs.result;
            // Only clear register status if this RS still owns it
            if (registerStatus[rs.destinationReg] == &rs) {
                registerStatus[rs.destinationReg] = nullptr;
            }
        }

        // Update any dependent reservation stations
        for (auto& otherRS : reservationStations) {
            if (!otherRS.Busy) continue;

            if (otherRS.Qj == rs.Name) {
                otherRS.Vj = rs.result;
                otherRS.Qj.clear();
            }
            if (otherRS.Qk == rs.Name) {
                otherRS.Vk = rs.result;
                otherRS.Qk.clear();
            }
        }

        // Handle special instructions
        if (rs.Op == "BEQ") {
            // Check branch condition and update PC if taken
            if (rs.Vj == rs.Vk) {  // Branch taken
                PC = rs.A;
                branchCount++;
                // Update branch prediction stats if needed
            }
        } else if (rs.Op == "CALL") {
            PC = rs.Vj;  // Jump to target address
        } else if (rs.Op == "RET") {
            PC = rs.Vj;  // Return to saved address
        }

        // Clear the reservation station
        rs.clear();
        rs.Write_status = true;
    }

    // Clear CDB after broadcast
    commonDataBus.busy = false;
}


