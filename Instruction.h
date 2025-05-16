#pragma once
#include <string>
using namespace std;

struct Instruction {
    string op;
    string rA, rB, rC;
    int offset;
    string label;

    Instruction(string o, string a, string b, string c, int off = 0, string lbl = "");
    void print() const;
    static Instruction fromLine(const string& line);
};
