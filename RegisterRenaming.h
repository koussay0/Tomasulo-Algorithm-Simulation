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

    // Returns the physical register name assigned to the logical register
    string rename(const string& logicalReg);

    // Returns the current mapping table (optional for debugging)
    unordered_map<string, string> getRenameTable() const;

    // Resets the renaming table
    void reset();
};
