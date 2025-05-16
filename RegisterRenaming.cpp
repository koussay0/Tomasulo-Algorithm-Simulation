#include "RegisterRenaming.h"

RegisterRenaming::RegisterRenaming() : nextPhysical(0) {}

std::string RegisterRenaming::rename(const std::string& reg) {
    if (renameTable.find(reg) == renameTable.end()) {
        renameTable[reg] = "P" + std::to_string(nextPhysical++);
    }
    return renameTable[reg];
}