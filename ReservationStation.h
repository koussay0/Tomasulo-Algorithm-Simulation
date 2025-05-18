//
// Created by ismai on 5/18/2025.
//

#ifndef C_RESERVATIONSTATION_H
#define C_RESERVATIONSTATION_H
#include <cstdint>
#include <string>
using namespace std;

class ReservationStation{
public:
    string Name;
    bool Busy;
    string Op;
    int16_t Vj;
    int16_t Vk;
    string Qj;
    string Qk;
    int16_t A;

    ReservationStation(const string& name);
    bool isBusy();
    bool isEmpty();
    bool isReady();
    void issue(const string& op, int16_t vj, const string& qj, int16_t vk, const string& qk, int16_t address);
    void execute();
    void write();
    void clear();

};

#endif //C_RESERVATIONSTATION_H
