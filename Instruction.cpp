#include "Instruction.h"
#include <iostream>
#include <sstream>

Instruction::Instruction(std::string o, std::string a, std::string b, std::string c, int off, std::string lbl)
    : op(o), rA(a), rB(b), rC(c), offset(off), label(lbl) {}

void Instruction::print() const {
    std::cout << op << " " << rA << ", " << rB << ", " << rC << " Offset: " << offset << " Label: " << label << std::endl;
}

Instruction Instruction::fromLine(const std::string& line) {
    std::stringstream ss(line);
    std::string op, a, b, c;
    int off = 0;
    ss >> op;
    if (op == "LOAD" || op == "STORE") {
        std::string addr;
        ss >> a >> addr;
        size_t lparen = addr.find('(');
        size_t rparen = addr.find(')');
        off = std::stoi(addr.substr(0, lparen));
        b = addr.substr(lparen + 1, rparen - lparen - 1);
        return Instruction(op, a, b, "", off);
    } else if (op == "BEQ") {
        ss >> a >> b >> off;
        return Instruction(op, a, b, "", off);
    } else if (op == "CALL") {
        ss >> off;
        return Instruction(op, "", "", "", off);
    } else if (op == "RET") {
        return Instruction(op, "", "", "", 0);
    } else {
        ss >> a >> b >> c;
        return Instruction(op, a, b, c);
    }
}