#include "Simulator.h"

int main() {
    Simulator sim;
    sim.loadProgram("instructions.txt");
    sim.loadData("data.txt");
    sim.run();
    sim.printResults();
    return 0;
}

