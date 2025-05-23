#include "TomasuloSimulator.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <instructions.txt> <data.txt>\n";
        return 1;
    }
    TomasuloSimulator sim(argv[1], argv[2]);
    sim.simulate();
    sim.printResults();
    return 0;
}
