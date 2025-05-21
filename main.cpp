#include <iostream>
#include <unordered_map>
#include "Memory.h"
#include "TomasuloSimulator.h"
using namespace std;

//Purpose of main is to:
// 1) Initialize the memory
// 2) Declare the reservation stations + delay of each instruction
// 3) Parse the instructions
// 4) Call Tomasulo (start simulation)

int main() {
    //Initializing the memory
    Memory m1;
    m1.write(0, 10); //mem[0] = 10
    m1.write(1, 5); //mem[1] = 5

    // Declaring the reservation stations + delay of each instruction
    unordered_map<string, int> numOfStations;
    unordered_map<string, int> instructionLatency;
    numOfStations["LOAD"] = 2;
    numOfStations["STORE"] = 2;
    numOfStations["BEQ"] = 2;
    numOfStations["CALL/RET"] = 1; //consider making one for CALL and one for RET
    numOfStations["ADD/SUB"] = 4; //consider making one for ADD and one for SUB
    numOfStations["NOR"] = 2;
    numOfStations["MUL"] = 2;

    instructionLatency["LOAD"] = 6;
    instructionLatency["STORE"] = 6;
    instructionLatency["BEQ"] = 1;
    instructionLatency["CALL/RET"] = 1; //consider making one for CALL and one for RET
    instructionLatency["ADD/SUB"] = 2; //consider making one for ADD and one for SUB
    instructionLatency["NOR"] = 1;
    instructionLatency["MUL"] = 10;

    //Parsing the instruction
    string testcase1 = "C:/Users/ismai/OneDrive/Desktop/testcase1.txt";
    TomasuloSimulator ts (testcase1);


    //Starting the simulation

    ts.simulate(); //update simulate function parameters to allow for passing numOfStations and instructionLatency
    ts.printResults();

    return 0;



}
