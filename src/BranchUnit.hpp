#ifndef BRANCH_UNIT_
#define BRANCH_UNIT_

#include "Register.hpp"
#include "Section.hpp"
#include <string>
#include <vector>
#include "Instruction.hpp"

class BranchUnit {
 
 private:
  int find(const std::string&, std::vector<Section_t>& sections);

 public:
  int callCycles;
  int jmpCycles;
  int jeCycles;
  int jaCycles;
  int jbCycles;
  
  BranchUnit();
  void jump(int& PC, Instruction& currentInstruction,
    std::vector<Section_t>& sections);
  void jumpEqual(int& PC, Instruction& currentInstruction,
    int& flag, std::vector<Section_t>& sections, int& cycles);
  void jumpAbove(int& PC, Instruction& currentInstruction,
    int& flag, std::vector<Section_t>& sections, int& cycles);
  void jumpBelow(int& PC, Instruction& currentInstruction,
    int& flag, std::vector<Section_t>& sections, int& cycles);
  void call(int& PC, Instruction& currentInstruction,
    std::vector<int>& calls,
    std::vector<std::vector<Register_t>>& callRegister,
    std::vector<Register_t> registers,
    std::vector<Section_t>& sections);

};

#endif  // BRANCH_UNIT_