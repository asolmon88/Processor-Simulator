#ifndef LSUNIT_
#define LSUNIT_

#include <vector>
#include "Register.hpp"
#include "Instruction.hpp"

class LSUnit {

public:
 static void load(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);
 static void store(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);
 static void move(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);

};

#endif  // LSUNIT_