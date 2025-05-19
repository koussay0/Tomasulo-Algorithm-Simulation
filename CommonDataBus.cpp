#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "CommonDataBus.h"

CommonDataBus ::CommonDataBus() {
    value = 0;
    destinationReg = -1;
    busy = false;
}

void CommonDataBus :: broadcast(int16_t value, int destinationReg){
    this->value = value;
    this->destinationReg = destinationReg;
    this->busy = true;
}

void CommonDataBus ::clear() {
    this->value = 0;
    this->destinationReg = -1;
    this->busy = false;
}


bool CommonDataBus ::isBusy() {
    return busy;
}

int16_t CommonDataBus::getValue() {
    return value;
}

int CommonDataBus::getDestinationReg()  {
    return destinationReg;
}

