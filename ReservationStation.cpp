#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "ReservationStation.h"

ReservationStation ::ReservationStation(const std::string &name) {
    Name = name;
    Busy = false;
    Op = "";
    Vj = 0;
    Vk = 0;
    Qj = "";
    Qk = "";
    A = 0;
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

void ReservationStation ::issue(const string& op, int16_t vj, const string& qj, int16_t vk, const string& qk, int16_t address) {
    Busy = true;
    Op = op;
    Vj = vj;
    Qj = qj;
    Vk = vk;
    Qk = qk;
    A = address;
}

void ReservationStation::execute() {

}

void ReservationStation::write() {

}

void ReservationStation ::clear() {
    Busy = false;
    Op = "";
    Vj = 0;
    Vk = 0;
    Qj = "";
    Qk = "";
    A = 0;
}
