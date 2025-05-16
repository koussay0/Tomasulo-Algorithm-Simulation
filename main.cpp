#include "instruction.h"
#include<iostream>

int main() {
    TomasuloSimulator simulator(".\instructions.txt");
    simulator.simulate();
    simulator.printResults();
    return 0;
}
