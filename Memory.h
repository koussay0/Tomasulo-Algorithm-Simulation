#pragma once
#include <unordered_map>
#include <string>
using namespace std;
class Memory {
private:
    std::unordered_map<int, int> data;

public:
    void loadData(const string& filename);
    int read(int address);
    void write(int address, int value);
};