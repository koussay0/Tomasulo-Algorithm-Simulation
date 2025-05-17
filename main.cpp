#include "instruction.h"
#include<iostream>

int main() {
    std::string fullPath = "C:/Users/kouss/CLionProjects/untitled/instructions.txt";
    TomasuloSimulator simulator(fullPath);

    simulator.simulate();
    simulator.printResults();

    return 0;
}
