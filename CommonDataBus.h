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
    string sourceTag;

    CommonDataBus();
    void broadcast(int16_t value, int destinationReg, const string& sourceTag);
    void clear();
    bool isBusy();
};



#endif //C_COMMONDATABUS_H
