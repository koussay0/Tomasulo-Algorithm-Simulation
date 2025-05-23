//
// Created by ismai on 5/18/2025.
//

#ifndef C_RESERVATIONSTATION_H
#define C_RESERVATIONSTATION_H
#include <cstdint>
#include <string>
#include "FunctionalUnit.h"
using namespace std;

class ReservationStation{
public:
    string Name;
    bool Busy;
    bool Issue_status;
    bool Execute_status;
    bool Write_status;
    string Op;
    int16_t Vj;
    int16_t Vk;
    string Qj;
    string Qk;
    int16_t A;
    int instructionIndex;
    int destinationReg;
    FunctionalUnit* functionalUnit;
    int16_t result;

    ReservationStation() : Busy(false), Issue_status(false), Execute_status(false), 
                          Write_status(false), Vj(0), Vk(0), A(0), 
                          instructionIndex(-1), destinationReg(-1), 
                          functionalUnit(nullptr), result(0) {}
    ReservationStation(const string& name);
    bool isBusy();
    bool isEmpty();
    bool isReady();
    bool isIssued();
    bool isExecuted();
    bool isWriting();
    void setNextStatus();
    void displayStatus();
    void clear();
    void setFunctionalUnit(FunctionalUnit* fu);

};

#endif //C_RESERVATIONSTATION_H
