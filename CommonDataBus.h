//
// Created by ismai on 5/17/2025.
//

#ifndef C_COMMONDATABUS_H
#define C_COMMONDATABUS_H
#include <string>
#include <cstdint>
using namespace std;

class CommonDataBus{
public:
    int16_t value;
    int destinationReg;
    bool busy;

    CommonDataBus();
    void broadcast(int16_t value, int destinationReg);
    void clear();
    bool isBusy();
    int16_t getValue();
    int getDestinationReg();
};



#endif //C_COMMONDATABUS_H
