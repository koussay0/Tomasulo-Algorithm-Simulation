#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "ReservationStation.h"

ReservationStation ::ReservationStation(const std::string &name) {
    Name = name;
    Busy = false;
    Issue_status = false;
    Execute_status = false;
    Write_status = false;
    Op = "";
    Vj = 0;
    Vk = 0;
    Qj = "";
    Qk = "";
    A = 0;
    destinationReg = -1;
}

bool ReservationStation ::isBusy() {
    return Busy;
}

bool ReservationStation::isEmpty() {
    if(Op == "")
        return true;
    return false;
}

bool ReservationStation ::isReady() {
    return Qj.empty() && Qk.empty();
}

bool ReservationStation ::isIssued() {
    return Issue_status;
}

bool ReservationStation::isExecuted() {
    return Execute_status;
}

bool ReservationStation::isWriting() {
    return  Write_status;
}

void ReservationStation::setNextStatus() {
    if (!Issue_status) {
        Issue_status = true;
    }
    else if (Issue_status && !Execute_status) {
        Execute_status = true;
    }
    else if (Execute_status && !Write_status) {
        Write_status = true;
    }
}

void ReservationStation ::displayStatus() {
    if(Issue_status)
        cout << "Issuing:" << endl;
    else if (Execute_status)
        cout << "Executing: " << endl;
    else if (Write_status)
        cout << "Writing: " << endl;
    else
        cout << "Invalid" << endl;
}

void ReservationStation ::clear() {
    Busy = false;
    Issue_status = false;
    Execute_status = false;
    Write_status = false;
    Op = "";
    Vj = 0;
    Vk = 0;
    Qj = "";
    Qk = "";
    A = 0;
    functionalUnit = nullptr;
}

void ReservationStation::setFunctionalUnit(FunctionalUnit* fu) {
    functionalUnit = fu;
}





