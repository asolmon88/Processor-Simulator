#ifndef ALU_
#define ALU_

#include <vector>

#include "Register.hpp"
#include "Instruction.hpp"


class ALU {
  public:
    int multiplyCycles;
    int compareCycles;

    ALU();
   void add(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   void substract(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   void multiply(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   void compare(std::vector<Register_t>& registers,
    Instruction& currentInstruction, int& flag);
};

#endif  // ALU_
