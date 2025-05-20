#include "RegisterRenaming.h"
#include <sstream>

RegisterRenaming::RegisterRenaming() {
    nextPhysical = 0;
    renameTable.clear();
}

// Returns the physical register assigned to the logical register
string RegisterRenaming::rename(const string& logicalReg) {
    // Check if already renamed
    if (renameTable.find(logicalReg) != renameTable.end()) {
        return renameTable[logicalReg];
    }

    // Generate new physical register name
    stringstream ss;
    ss << "P" << nextPhysical++; // Physical register naming: P0, P1, P2, ...

    string physicalReg = ss.str();
    renameTable[logicalReg] = physicalReg;
    return physicalReg;
}

unordered_map<string, string> RegisterRenaming::getRenameTable() const {
    return renameTable;
}

void RegisterRenaming::reset() {
    renameTable.clear();
    nextPhysical = 0;
}
