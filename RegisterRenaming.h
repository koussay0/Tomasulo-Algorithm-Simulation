#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class RegisterRenaming {
private:
    unordered_map<string, string> renameTable;
    int nextPhysical;

public:
    RegisterRenaming();
    string rename(const string& reg);
};