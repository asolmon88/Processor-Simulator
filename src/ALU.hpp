#ifndef ALU_
#define ALU_
#include "Simulator.hpp"
#include "Register.hpp"
#include "Instruction.hpp"

class ALU {
  public:
   static void add(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   static void substract(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   static void multiply(std::vector<Register_t>& registers,
    Instruction& currentInstruction);
   static void compare(std::vector<Register_t>& registers,
    Instruction& currentInstruction, int& flag);
};

#endif  // ALU_
