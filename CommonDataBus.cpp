#include <iostream>
#include <string> // for std::memcpy
#include <cstdint>
using namespace std;
#include "CommonDataBus.h"

CommonDataBus ::CommonDataBus() {
    value = 0;
    destinationReg = -1;
    busy = false;
    sourceTag = "";
}

void CommonDataBus :: broadcast(int16_t value, int destinationReg, const std::string& sourceTag){
    this->value = value;
    this->destinationReg = destinationReg;
    this->sourceTag = sourceTag;
    this->busy = true;
}

void CommonDataBus ::clear() {
    this->value = 0;
    this->destinationReg = -1;
    this->sourceTag = "";
    this->busy = false;
}


bool CommonDataBus ::isBusy() {
    return this->busy;
}