#ifndef LSUNIT_
#define LSUNIT_

#include <vector>
#include "Register.hpp"
#include "Instruction.hpp"

class LSUnit {

 public:
  int loadCycles;
  int moves;
  LSUnit();
  void load(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);
  void store(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);
  void move(std::vector<Register_t>& registers,
    Instruction& currentInstruction, std::vector<int>& memory);

};

#endif  // LSUNIT_